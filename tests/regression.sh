#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
TMPDIR=$(mktemp -d)
trap 'rm -rf "$TMPDIR"' EXIT

cd "$ROOT"
make >/dev/null

printf 'is md5("salt") a salted hash?\n' > "$TMPDIR/file"
./ft_ssl md5 "$TMPDIR/file" | awk '{print $NF}' > "$TMPDIR/ft_md5"
md5sum --tag "$TMPDIR/file" | awk '{print $NF}' > "$TMPDIR/sys_md5"
diff "$TMPDIR/ft_md5" "$TMPDIR/sys_md5"

printf 'Magic mirror on the wall, think I wanna smash them all?\n' | ./ft_ssl md5 -p > "$TMPDIR/p_out"
[ -s "$TMPDIR/p_out" ]

printf 'Lorem ipsum dolor amet thundercats letterpress cray portland cornhole\n' > "$TMPDIR/ipsum"
./ft_ssl sha256 -q "$TMPDIR/ipsum" > "$TMPDIR/ft_sha"
openssl sha256 "$TMPDIR/ipsum" | awk '{print $2}' > "$TMPDIR/sys_sha"
diff "$TMPDIR/ft_sha" "$TMPDIR/sys_sha"

./ft_ssl foobar > /dev/null 2> "$TMPDIR/invalid_err" || true
grep -q "invalid command" "$TMPDIR/invalid_err"

printf 'md5 -s "foo bar"\nsha256 -s foo\n' | ./ft_ssl > "$TMPDIR/stdin_out"
grep -q 'MD5 ("foo bar")' "$TMPDIR/stdin_out"
grep -q 'SHA256 ("foo")' "$TMPDIR/stdin_out"

echo "regression checks passed"
