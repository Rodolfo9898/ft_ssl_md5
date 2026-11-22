#!/bin/bash
# Mandatory checks (make, then ./ft_ssl md5 and sha256). Exit status: the
# number of failed checks.

source "$(dirname "$0")/setup.sh"

echo "### make"
build all
cd "$OUT" || exit 1

echo "### usage and invalid commands (ft_ssl_md5.pdf p.7)"
$SSL </dev/null >run.out 2>run.err
code=$?
same "no command: the usage, on stderr" \
	"usage: ft_ssl command [flags] [file/string]" "$(cat run.out run.err)"
[ "$code" -ne 0 ] && [ ! -s run.out ]
check $? "no command: status $code, nothing on stdout"
INVALID="ft_ssl: Error: 'foobar' is an invalid command.
Commands:
md5
sha256
Flags:
-p -q -r -s"
same "foobar: the error and the commands" "$INVALID" "$($SSL foobar 2>&1)"
$SSL foobar >/dev/null 2>&1
check $([ $? -ne 0 ]; echo $?) "foobar: status not 0"
$SSL whirlpool -s abc 2>&1 | grep -q "'whirlpool' is an invalid command"
check $? "whirlpool is a bonus command: make does not have it"

echo "### the examples of the subject (md5 p.9, sha256 p.10)"
same 'echo "42 is nice" | md5' "(stdin)= 35f1d6de0302e2086a4e472266efb3a9" \
	"$(echo "42 is nice" | $SSL md5 2>&1)"
same 'echo "42 is nice" | md5 -p' '("42 is nice")= 35f1d6de0302e2086a4e472266efb3a9' \
	"$(echo "42 is nice" | $SSL md5 -p 2>&1)"
same 'echo "Pity the living." | md5 -q -r' "e20c3b973f63482a778f3fd1869b7f25" \
	"$(echo "Pity the living." | $SSL md5 -q -r 2>&1)"
same "md5 file" "MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a" "$($SSL md5 file 2>&1)"
same "md5 -r file" "53d53ea94217b259c11a5a2d104ec58a file" "$($SSL md5 -r file 2>&1)"
same 'md5 -s "pity those..."' \
	"MD5 (\"pity those that aren't following baerista on spotify.\") = a3c990a1964705d9bf0e602f44572f5f" \
	"$($SSL md5 -s "pity those that aren't following baerista on spotify." 2>&1)"
same "md5 -p file" '("be sure to handle edge cases carefully")= 3553dc7dc5963b583c056d1b9fa3349c
MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a' \
	"$(echo "be sure to handle edge cases carefully" | $SSL md5 -p file 2>&1)"
same "md5 file, with stdin: stdin is not read" "MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a" \
	"$(echo "some of this will not make sense at first" | $SSL md5 file 2>&1)"
same "md5 -p -r file" '("but eventually you will understand")= dcdd84e0f635694d2a943fa8d3905281
53d53ea94217b259c11a5a2d104ec58a file' \
	"$(echo "but eventually you will understand" | $SSL md5 -p -r file 2>&1)"
same 'md5 -p -s "foo" file' '("GL HF let'"'"'s go")= d1e3cc342b6da09480b27ec57ff243e2
MD5 ("foo") = acbd18db4cc2f85cedef654fccc4a4d8
MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a' \
	"$(echo "GL HF let's go" | $SSL md5 -p -s "foo" file 2>&1)"
same 'md5 -r -p -s "foo" file -s "bar": after a file, only files' '("one more thing")= a0bd1876c6f011dd50fae52827f445f5
acbd18db4cc2f85cedef654fccc4a4d8 "foo"
53d53ea94217b259c11a5a2d104ec58a file
ft_ssl: md5: -s: No such file or directory
ft_ssl: md5: bar: No such file or directory' \
	"$(echo "one more thing" | $SSL md5 -r -p -s "foo" file -s "bar" 2>&1)"
same 'md5 -r -q -p -s "foo" file' 'just to be extra clear
3ba35f1ea0d170cb3b9a752e3360286c
acbd18db4cc2f85cedef654fccc4a4d8
53d53ea94217b259c11a5a2d104ec58a' \
	"$(echo "just to be extra clear" | $SSL md5 -r -q -p -s "foo" file 2>&1)"
same "sha256 -q website" "1ceb55d2845d9dd98557b50488db12bbf51aaca5aa9c1199eb795607a2457daf" \
	"$($SSL sha256 -q website 2>&1)"
same 'sha256 -s "42 is nice"' \
	'SHA256 ("42 is nice") = b7e44c7a40c5f80139f0a50f3650fb2bd8d00b0d24667c4c2ca32c88e13b758f' \
	"$($SSL sha256 -s "42 is nice" 2>&1)"
same "sha256 -p -q -r -s foo file: the same flags as md5" "$(printf 'hi\n%s\n%s\n%s' \
	98ea6e4f216f2fb4b69fff9b3a44842c38686ca685f3f55dc48c5d3fb1107be4 \
	2c26b46b68ffc68ff99b453c1d30413413422d706483bfa0f98a5e886266e7ae \
	f9eb9a5a063eb386a18525c074e1065c316ec434f911e0d7d59ba2d9fd134705)" \
	"$(echo hi | $SSL sha256 -q -r -p -s foo file 2>&1)"

echo "### digests against md5sum and sha256sum"
bad_md5=0
bad_sha=0
for n in $(seq 0 130) 1000 4095 4096 4097 65537 1048576 20000000; do
	head -c "$n" /dev/urandom >random.bin
	[ "$($SSL md5 -q random.bin)" == "$(md5sum random.bin | cut -d' ' -f1)" ] \
		|| { bad_md5=$((bad_md5 + 1)); echo "        md5: $n bytes differ"; }
	[ "$($SSL sha256 -q random.bin)" == "$(sha256sum random.bin | cut -d' ' -f1)" ] \
		|| { bad_sha=$((bad_sha + 1)); echo "        sha256: $n bytes differ"; }
done
check $bad_md5 "md5: random files of 0 to 130 bytes (every padding case), 1 KB to 20 MB"
check $bad_sha "sha256: the same files"
same "md5 of stdin with NUL bytes" "$(printf 'a\0b' | md5sum | cut -d' ' -f1)" \
	"$(printf 'a\0b' | $SSL md5 -q)"
same "md5 of an empty stdin" "(stdin)= d41d8cd98f00b204e9800998ecf8427e" "$(printf '' | $SSL md5)"
same 'md5 -s ""' 'MD5 ("") = d41d8cd98f00b204e9800998ecf8427e' "$($SSL md5 -s '')"
same 'sha256 -r -s "" (quotes kept)' \
	'e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855 ""' "$($SSL sha256 -r -s '')"

echo "### errors: a reason on stderr, status not 0, and stdin is never read for them"
mkdir -p a_directory
for target in no_such_file a_directory; do
	run_open_stdin $SSL md5 "$target"
	[ "$CODE" -ne 0 ] && [ "$CODE" -ne 124 ] && [ -z "$STDOUT" ] \
		&& echo "$STDERR" | grep -q "ft_ssl: md5: $target: "
	check $? "md5 $target -> ${STDERR:-nothing on stderr} (status $CODE)"
done
if [ "$(id -u)" -ne 0 ]; then
	echo secret >unreadable && chmod 000 unreadable
	run_open_stdin $SSL sha256 unreadable
	[ "$CODE" -ne 0 ] && [ "$CODE" -ne 124 ] && [ -z "$STDOUT" ] \
		&& echo "$STDERR" | grep -q "Permission denied"
	check $? "sha256 unreadable -> ${STDERR:-nothing on stderr} (status $CODE)"
	chmod 600 unreadable
else
	note "running as root: the unreadable file check is skipped"
fi
run_open_stdin $SSL md5 no_such_file file
[ "$CODE" -eq 1 ] && [ "$STDOUT" == "MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a" ]
check $? "md5 no_such_file file: the next file is still hashed (status $CODE)"
run_open_stdin $SSL md5 -s
[ "$CODE" -ne 0 ] && [ "$CODE" -ne 124 ] && [ -n "$STDERR" ] && [ -z "$STDOUT" ]
check $? "md5 -s without its string -> ${STDERR:-nothing on stderr} (status $CODE)"
$SSL md5 <&- >run.out 2>run.err
code=$?
[ "$code" -ne 0 ] && [ -s run.err ]
check $? "md5 with stdin closed -> $(head -c 80 run.err) (status $code)"
if [ -w /dev/full ]; then
	$SSL md5 -s abc >/dev/full 2>run.err
	code=$?
	[ "$code" -ne 0 ] && grep -q "stdout" run.err
	check $? "md5 -s abc >/dev/full -> $(head -c 80 run.err) (status $code)"
fi

echo "### leaks (valgrind)"
echo "some input" >stdin.txt
leak_check "md5 -p -s foo file no_such_file -s bar" stdin.txt $SSL md5 -p -s foo file no_such_file -s bar
leak_check "sha256 -r -q -p file" stdin.txt $SSL sha256 -r -q -p file
leak_check "md5 (stdin)" stdin.txt $SSL md5
leak_check "md5 a_directory" stdin.txt $SSL md5 a_directory
leak_check "md5 -s (missing string)" stdin.txt $SSL md5 -s
leak_check "foobar" stdin.txt $SSL foobar
leak_check "no command" stdin.txt $SSL

echo "### mandatory: $FAILS failed check(s)"
exit "$FAILS"
