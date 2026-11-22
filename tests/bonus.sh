#!/bin/bash
# Bonus checks (make bonus, then ./ft_ssl_bonus): the command reader of the
# standard input, and whirlpool. Exit status: the number of failed checks.

source "$(dirname "$0")/setup.sh"
SSL=$ROOT/ft_ssl_bonus

echo "### make bonus"
build bonus
cd "$OUT" || exit 1

echo "### commands read from stdin (ft_ssl_md5.pdf p.12)"
same "the piped example of the subject" \
	'MD5 ("42 is nice") = 0029a98ee90fdb85d70924d44d3c9e75
SHA256 ("42 is nice") = b7e44c7a40c5f80139f0a50f3650fb2bd8d00b0d24667c4c2ca32c88e13b758f' \
	"$(printf 'md5 -s "42 is nice"\nsha256 -s "42 is nice"\n' | timeout 5 $SSL 2>&1)"
same "the session of the subject: foobar keeps reading, exit ends it" \
	"MD5 (\"42 is nice\") = 0029a98ee90fdb85d70924d44d3c9e75
1ceb55d2845d9dd98557b50488db12bbf51aaca5aa9c1199eb795607a2457daf
ft_ssl: Error: 'foobar' is an invalid command.
Commands:
md5
sha256
whirlpool
Flags:
-p -q -r -s" \
	"$(printf 'md5 -s "42 is nice"\nsha256 -q website\nfoobar\nexit\nmd5 -s "not run"\n' \
		| timeout 5 $SSL 2>&1)"
same "quit ends it too" "" "$(printf 'quit\nmd5 -s "not run"\n' | timeout 5 $SSL 2>&1)"
printf 'md5 -s a\n' | timeout 5 $SSL >/dev/null 2>&1
check $? "the end of the input (Ctrl-D) ends it, status 0"
same "empty and blank lines do nothing" 'MD5 ("x") = 9dd4e461268c8034f5c8564e155c67a6' \
	"$(printf '\n   \n\t\nmd5 -s x\n\n' | timeout 5 $SSL 2>&1)"
same "the last line runs without its newline" 'MD5 ("x") = 9dd4e461268c8034f5c8564e155c67a6' \
	"$(printf 'md5 -s x' | timeout 5 $SSL 2>&1)"
for line in 'md5 -r -s "foo" file -s "bar"' "sha256 -q -s 'single quoted' file" \
	'md5 -s a\ b no_such_file' 'whirlpool -r -s "foo" file'; do
	eval "set -- $line"
	same "a line runs as its arguments would: $line" "$($SSL "$@" 2>&1)" \
		"$(echo "$line" | timeout 5 $SSL 2>&1)"
done
printf 'foobar\n' | timeout 5 $SSL >/dev/null 2>&1
code=$?
[ "$code" -ne 0 ] && [ "$code" -ne 124 ]
check $? "a failed command makes the final status not 0 (status $code)"

echo "### the mandatory part still works with make bonus"
same "md5 -p -s foo file" '("hi")= 764efa883dda1e11db47671c4a3bbd9e
MD5 ("foo") = acbd18db4cc2f85cedef654fccc4a4d8
MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a' "$(echo hi | $SSL md5 -p -s foo file 2>&1)"
run_open_stdin $SSL sha256 no_such_file
[ "$CODE" -eq 1 ] && [ -z "$STDOUT" ]
check $? "sha256 no_such_file: an error, and stdin is not read (status $CODE)"

echo "### whirlpool"
same 'whirlpool -s "" (ISO test vector)' \
	'WHIRLPOOL ("") = 19fa61d75522a4669b44e39c1d2e1726c530232130d407f89afee0964997f7a73e83be698b288febcf88e3e03c4f0757ea8964e59b63d93708b138cc42a66eb3' \
	"$($SSL whirlpool -s '' 2>&1)"
same 'whirlpool -q -s "abc" (ISO test vector)' \
	'4e2448a4c6f486bb16b6562c73b4020bf3043e3a731bce721ae1b303d97e6d4c7181eebdb6c57e277d0e34957114cbd6c797fc9d95d8b582d225292076d4eef5' \
	"$($SSL whirlpool -q -s abc 2>&1)"
WP=$($SSL whirlpool -q file)
same "whirlpool -p -r -s foo file: the four flags" "(\"hi\")= $(echo hi | $SSL whirlpool -q)
$($SSL whirlpool -q -s foo) \"foo\"
$WP file" "$(echo hi | $SSL whirlpool -p -r -s foo file 2>&1)"
same "whirlpool file" "WHIRLPOOL (file) = $WP" "$($SSL whirlpool file 2>&1)"
same "whirlpool, stdin" "(stdin)= $WP" "$($SSL whirlpool <file 2>&1)"
if openssl whirlpool -provider legacy -provider default /dev/null >/dev/null 2>&1; then
	bad=0
	for n in $(seq 0 130) 1000 4095 4096 4097 65537 1048576 5000000; do
		head -c "$n" /dev/urandom >random.bin
		[ "$($SSL whirlpool -q random.bin)" == \
			"$(openssl whirlpool -provider legacy -provider default -r random.bin | cut -d' ' -f1)" ] \
			|| { bad=$((bad + 1)); echo "        whirlpool: $n bytes differ"; }
	done
	check $bad "whirlpool against openssl: random files of 0 to 130 bytes, 1 KB to 5 MB"
else
	note "openssl has no whirlpool here (its legacy provider): the comparison is skipped"
fi

echo "### leaks (valgrind)"
printf 'md5 -s "42 is nice"\nsha256 -q website\nfoobar\n\n  \nmd5 -s "unterminated\nwhirlpool -s a\\ b file\nmd5 no_such_file\nexit\n' >session.txt
leak_check "a session: commands, foobar, blank lines, quotes, exit" session.txt $SSL
printf 'md5 -s x' >no_newline.txt
leak_check "a session ended by the end of the input" no_newline.txt $SSL
echo "some input" >stdin.txt
leak_check "whirlpool -p -r -s foo file no_such_file" stdin.txt $SSL whirlpool -p -r -s foo file no_such_file

echo "### bonus: $FAILS failed check(s)"
exit "$FAILS"
