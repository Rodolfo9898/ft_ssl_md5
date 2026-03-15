# ft_ssl_md5 TODO

## Completed

- Hardened digest computation failure handling.
  - MD5 and SHA-256 now report allocation failures cleanly instead of printing uninitialized digest data.
  - The shared digest path now stops on compute failure and emits a sensible system error.

- Improved file and stream error reporting.
  - File open/read failures now use real system error messages.
  - Buffer growth failures propagate as `ENOMEM`.

- Cleaned evaluation artifacts.
  - Removed temporary test files from the repository tree.
  - Removed the stale `src/commands/digest/digest_algorithms.c` file.

- Reduced dispatcher risk.
  - Command listings now come from the registry rather than duplicating command names in output code.

- Added evaluator regression coverage.
  - `tests/regression.sh` checks invalid commands, md5/sha256 correctness, flag behavior, and stdin command parsing.

- Implemented Bonus 1.
  - `./ft_ssl` can now read commands from STDIN, tokenize them, and dispatch them through the same registry.

- Implemented Bonus 2.
  - Added a `whirlpool` digest command.
  - Verified `printf abc | ./ft_ssl whirlpool` matches OpenSSL Whirlpool output.

## Notes

- Build and verification were completed successfully during this session.
- Run `make` to rebuild the binary.
- Run `./tests/regression.sh` for a quick local verification pass.
