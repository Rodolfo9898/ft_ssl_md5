# ft_ssl_md5 TODO

## Mandatory blockers

- Fix allocation failure handling in the digest path.
  - `src/commands/digest/md5.c`: `md5_compute()` returns early if `pad_message()` fails, but does not report failure to the caller.
  - `src/commands/digest/sha256.c`: `sha256_compute()` has the same issue.
  - `src/commands/digest/digest_command.c`: `compute_and_print()` always prints the digest buffer, so an allocation failure can currently print uninitialized data.
  - Needed change: make digest computation return success/failure and print a sensible error on failure instead of emitting a hash.

- Improve file read error reporting.
  - `src/commands/digest/digest_command.c`: `process_file()` treats every read/open failure the same way.
  - `src/core/output.c`: `print_digest_error()` hardcodes `No such file or directory`.
  - Needed change: either use `errno` for a real system error message, or at minimum distinguish open/read failures more sensibly.

- Remove temporary test artifacts before submission.
  - Current stray file: `file`

## Bonus exploration

- Bonus 1: parse commands from STDIN like OpenSSL.
  - Add an interactive mode when `./ft_ssl` is launched without a command.
  - Read one command line at a time from STDIN.
  - Tokenize command + args and dispatch through the same command registry.
  - Keep behavior compatible with existing non-interactive command execution.
  - Likely needed:
    - a reusable argument tokenizer
    - a command loop
    - careful quote/escape handling if OpenSSL-style input parsing is expected

- Bonus 2: implement a stronger hash than MD5.
  - Maximum points require `whirlpool`.
  - Add a new digest algorithm module and register a new command.
  - Reuse the existing digest flag/output layer (`-p`, `-q`, `-r`, `-s`).
  - Likely needed:
    - Whirlpool block transform
    - Whirlpool padding/finalization
    - test vectors for validation

## Suggested order

1. Fix mandatory blockers first.
2. Add regression tests for subject examples and error paths.
3. Only then start Bonus 1 interactive parsing.
4. Add Whirlpool last, after the digest compute interface is made failure-aware.
