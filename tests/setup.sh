#!/bin/bash
# Shared helpers, sourced by tests/mandatory.sh and tests/bonus.sh. Logs and the
# files the checks hash go to test_out/. Run from the root of the repository:
# each script builds the program it tests itself (make for ./ft_ssl, make bonus
# for ./ft_ssl_bonus, which bonus.sh sets in SSL).

# Bytes, not characters.
export LC_ALL=C

ROOT=$(pwd)
OUT=$ROOT/test_out
SSL=$ROOT/ft_ssl
FAILS=0
rm -rf "$OUT"
mkdir -p "$OUT"

check() { # $1 = condition result (0 = ok), $2 = description
	if [ "$1" -eq 0 ]; then
		echo "  PASS  $2"
	else
		echo "  FAIL  $2"
		FAILS=$((FAILS + 1))
	fi
}

note() { echo "  NOTE  $1"; }

build() { # $1 = the make rule: all or bonus, which builds $SSL; stops the script when it fails
	make --no-print-directory "$1" >"$OUT/make_$1.log" 2>&1
	check $? "make $1 builds ./$(basename "$SSL")"
	if [ ! -x "$SSL" ]; then
		echo "### no ./$(basename "$SSL"): the other checks cannot run (see $OUT/make_$1.log)"
		exit "$FAILS"
	fi
}

# What ft_ssl prints, standard output then standard error, against what is
# expected; the difference is shown when they differ. Run in $OUT, so that the
# files are named as in the subject: "file", "website".
same() { # $1 = description, $2 = expected, $3 = got
	if [ "$2" == "$3" ]; then
		check 0 "$1"
	else
		check 1 "$1"
		diff <(echo "$2") <(echo "$3") | sed 's/^/        /' | head -20
	fi
}

# Runs a command, its standard input from a pipe that stays open for 5
# seconds: a command that reads it waits, and is killed after 3 (status 124).
# Sets CODE, STDOUT and STDERR.
run_open_stdin() {
	sleep 5 | timeout 3 "$@" >"$OUT/run.out" 2>"$OUT/run.err"
	CODE=${PIPESTATUS[1]}
	STDOUT=$(cat "$OUT/run.out")
	STDERR=$(cat "$OUT/run.err")
}

# valgrind: no error, no leak of any kind, every block freed at exit.
leak_check() { # $1 = description, $2 = file for stdin, the rest = the command
	local desc=$1 input=$2
	shift 2
	if ! command -v valgrind >/dev/null; then
		note "valgrind is missing: $desc skipped"
		return
	fi
	valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all \
		--error-exitcode=42 --log-file="$OUT/leaks.log" "$@" <"$input" >/dev/null 2>&1
	[ $? -ne 42 ] && grep -q "ERROR SUMMARY: 0 errors" "$OUT/leaks.log" \
		&& grep -qE "in use at exit: 0 bytes|no leaks are possible" "$OUT/leaks.log"
	check $? "$desc"
}

# The files of the subject's examples.
echo "And above all," >"$OUT/file"
echo "https://www.42.fr/" >"$OUT/website"
