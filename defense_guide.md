# ft_ssl_md5 Defense Guide

## 1) FT_SSL basics
`ft_ssl` is a command dispatcher in C.
- If there are no CLI arguments, it reads commands from STDIN.
- If there are arguments, it executes the command from `argv[1]`.
- Invalid commands print a clear error and the list of commands.

## 2) Function dispatcher (important subjective point)
I use a command registry table with this structure:
- `name`: command string (`"md5"`, `"sha256"`, etc.)
- `group`: command family (digest/cipher/standard)
- `handler`: function pointer called for execution
- `data`: command-specific context (for example, which digest algorithm)

Then I use one lookup + one dispatch path:
1. `find_command(name)` searches the registry.
2. `execute_command_argv(...)` calls `command->handler(...)`.

There is no command `if/else` forest.
Adding a command means adding one row in the registry table.
The command list output also reads from the same table.

### Example: adding a new command
If I want to add `sha512` later:

1. Implement algorithm getter:
`const t_digest_algorithm *get_sha512_algorithm(void);`

2. Add one entry in command table:
```c
{
	"sha512",
	COMMAND_GROUP_MESSAGE_DIGEST,
	run_digest_command,
	(const void *)get_sha512_algorithm
}
```

3. Done: lookup, dispatch, and command listing work automatically.

## 3) MD5 implementation
MD5 is implemented from scratch:
- message padding
- little-endian block parsing
- 64 rounds
- digest output formatting

Validated against `md5sum`/OpenSSL outputs.

## 4) Flags `-p -q -r -s`
- `-p`: read stdin, echo it, then print hash
- `-q`: digest only
- `-r`: reverse output format
- `-s`: hash provided string argument

Same behavior model is shared by MD5 and SHA-256.

## 5) SHA-256 implementation
SHA-256 is implemented from scratch:
- schedule expansion
- 64 rounds
- big-endian digest output

Validated against `openssl sha256`.

## 6) Bonus points
- STDIN command parsing is supported.
- Whirlpool (stronger than MD5) is integrated.

## 7) Modularity / future projects
Project is split by responsibility:
- `src/commands/*` for commands
- shared digest pipeline
- per-algorithm folders (`md5`, `sha256`, `whirlpool`)
- `src/core/*` for IO, stdin parsing, output, registry

`objects/` mirrors `src/` structure, so extending to DES/RSA is easier.
