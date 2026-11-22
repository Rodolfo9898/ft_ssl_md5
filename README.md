*This project has been created as part of the 42 curriculum by rperez-t.*

# ft_ssl [md5] [sha256]

## Description

Cryptographic hashing from scratch. `ft_ssl` recodes part of OpenSSL: it hashes
strings, files and its standard input with **md5** and **sha256**, written from their
specifications, and prints the digest the way the subject shows it. It is the first
ft_ssl project, so the program is built to take more commands later (standard and
cipher commands) without being rewritten.

| Part | Program | What it does |
|---|---|---|
| Mandatory | `ft_ssl` | `md5` and `sha256`, with the flags `-p -q -r -s` |
| Bonus | `ft_ssl_bonus` | The same, plus commands read from the standard input and `whirlpool` |

The flags are those of the subject: `-p` echoes the standard input then prints its
digest, `-q` prints the digest alone, `-r` prints the digest before what was hashed,
and `-s` hashes the string that follows.

## Instructions

### Requirements

A Linux machine (the subject asks for one) with `cc`, `make` and `git`. The first
`make` fetches libft (see *How the libraries are handled*), which needs network access,
but no GitHub account or ssh key. The tests also use `valgrind`, `md5sum`, `sha256sum`
and `openssl` (with its legacy provider, for whirlpool); the Docker image has them all.

### Building

```sh
make        # ft_ssl
make bonus  # ft_ssl_bonus
```

| Rule | Effect |
|---|---|
| `all`, `ft_ssl` | Build `ft_ssl`, relinking only what changed |
| `bonus` | Build `ft_ssl_bonus`: the same files, with `-D BONUS_MODE=1` |
| `debug` | The mandatory build with `-g -fsanitize=address` (run `make fclean` first) |
| `clean` | Remove the objects, including those of libft |
| `fclean` | `clean`, and remove the programs and `libft.a` |
| `re` | `fclean`, then `all` |
| `docker-mandatory` | Run the mandatory checks in Docker (see *Testing on Linux with Docker*) |
| `docker-bonus` | Run the bonus checks in Docker |
| `docker-shell` | Open a bash shell in the Docker image, with both programs built |
| `docker-build` | Fetch the submodule and build the image (the three rules above do it for you) |
| `docker-clean` | Remove the Docker image |

### Running

```sh
echo "42 is nice" | ./ft_ssl md5            # (stdin)= 35f1d6de0302e2086a4e472266efb3a9
./ft_ssl md5 -s "42 is nice"                # MD5 ("42 is nice") = 0029a98ee90fdb85d70924d44d3c9e75
echo "And above all," > file
./ft_ssl sha256 -q file                     # f9eb9a5a063eb386a18525c074e1065c316ec434f911e0d7d59ba2d9fd134705
echo "GL HF let's go" | ./ft_ssl md5 -p -s "foo" file
```

```
("GL HF let's go")= d1e3cc342b6da09480b27ec57ff243e2
MD5 ("foo") = acbd18db4cc2f85cedef654fccc4a4d8
MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a
```

Without a command, `ft_ssl` prints `usage: ft_ssl command [flags] [file/string]`. An
unknown command prints `ft_ssl: Error: 'foobar' is an invalid command.` and the list of
commands and flags. The arguments are read in order:

- `-p` and `-s` are acted on where they stand, so a `-q` or `-r` only changes the lines
  after it (`-q -p` echoes the input bare, `-p -q` prints it as `("...")= `);
- from the first argument that is not a flag on, every argument is a file, even `-s`
  (the subject's example prints `ft_ssl: md5: -s: No such file or directory`);
- the standard input is read only when no file, `-s` or `-p` was given. A file that
  cannot be read is an error, never a reason to read the standard input.

On an error, `ft_ssl` prints `ft_ssl: <command>: <what>: <reason>` on the standard
error, goes on with the next files, and exits with status 1: a missing, unreadable or
directory file, `-s` without its string, a standard input that cannot be read, a result
that cannot be written (a full disk, for example).

### ft_ssl_bonus

`make bonus` builds `ft_ssl_bonus`, the same program with the two bonuses of the
subject. With arguments, it works like `ft_ssl`, with one more command, `whirlpool`,
which takes the same four flags and matches `openssl whirlpool`. Without a command, it
reads one command per line from its standard input:

```sh
printf 'md5 -s "42 is nice"\nsha256 -s "42 is nice"\n' | ./ft_ssl_bonus
```

```
MD5 ("42 is nice") = 0029a98ee90fdb85d70924d44d3c9e75
SHA256 ("42 is nice") = b7e44c7a40c5f80139f0a50f3650fb2bd8d00b0d24667c4c2ca32c88e13b758f
```

- A line runs exactly as if its words had been the arguments: `"..."` and `'...'` group
  words, `\` escapes the next character. An empty line does nothing, an unknown command
  prints the usual error and the reader goes on.
- `exit`, `quit` or the end of the input (Ctrl-D when typing) stop it. Its status is 1
  when one of the commands failed.
- There is no prompt (the subject leaves it to us), so piped input gives only the
  results. The reader reads one byte at a time, so that a command with `-p` can still
  read what follows its line on the standard input.

### Testing

```sh
bash tests/mandatory.sh
bash tests/bonus.sh
```

Each script builds the program it tests (`make` for `./ft_ssl`, `make bonus` for
`./ft_ssl_bonus`). The exit status of each script is its number of failed checks, and
logs go to `test_out/`.

`mandatory.sh` compares every md5 and sha256 example of the subject with its exact
output, then the two hashes with `md5sum` and `sha256sum` on random files of 0 to 130
bytes (every padding case) and up to 20 MB. Each error is run with a standard input that
stays open, so that a program that reads it instead of failing is caught. valgrind
checks valid and failing commands.

`bonus.sh` runs the two examples of the subject for the command reader, `exit`, `quit`
and the end of the input, blank lines, and lines compared with the same arguments given
on the command line. Whirlpool is checked on the ISO test vectors, with its four flags,
and against `openssl whirlpool` up to 5 MB. valgrind checks whole sessions.

### Testing on Linux with Docker

The `Dockerfile` builds both programs in Debian and runs the same checks there, so the
result does not depend on your machine, with valgrind for the leak checks. It runs them
as a user that is not root, so that an unreadable file stays unreadable.

```sh
make docker-mandatory                              # mandatory part only
make docker-bonus                                  # bonus part only
make docker-shell                                  # bash inside the image
make docker-build && docker run --rm ft_ssl-test   # mandatory, then bonus
make docker-clean                                  # remove the image
```

Each rule fetches the submodule and rebuilds the image first, so it always tests the
current sources. `make docker-mandatory` and `make docker-bonus` fail if any check
fails.

## How the libraries are handled

The subject allows functions that do not do the work for us, and libft only gives
string, memory and output helpers. A git submodule provides it at build time without
storing a second copy, so the library keeps a single source of truth.

| Path | Repository | Pinned commit |
|---|---|---|
| `libs/libft` | https://github.com/Rodolfo9898/libft | `0f4b7bf` |

- `git clone` leaves `libs/` empty. The first `make` runs
  `git submodule update --init --remote --recursive` (rule `submodules` in the
  `Makefile`), which puts libft at the newest commit of its `master` branch. It is then
  built by its own Makefile before the programs. `git clone --recurse-submodules`
  fetches it at clone time instead, at the commit stored here.
- The repository itself stores only the commit hash.
- The repository is public and the URL is `https`, so anyone can fetch it, with no
  GitHub account and no ssh key.
- Without network access, the first `make` stops with an error. In that case, fetch
  libft once on a connected machine and copy the whole folder.
- libft provides `ft_strlen`, `ft_memcpy`, `ft_bzero`, `ft_strncmp` and the error
  messages on the standard error (`ft_putstr_fd`, `ft_putendl_fd`). It only calls
  `malloc`, `free` and `write`.
- get_next_line is not used. The command reader of the bonus must not take more than its
  line from the standard input, which a command with `-p` reads next: get_next_line keeps
  what it reads past the line in its static buffer, unless `BUFFER_SIZE` is 1, which is
  the one-byte `read()` the reader already does. It also returns `NULL` both at the end
  of the input and on a read error, which the reader reports.

## Project structure

```
Makefile             rules only (all, bonus, clean, fclean, re, debug, submodules, docker-*)
mk/                  everything the Makefile includes
  colors.mk            the output's colours: commands green, errors red, warnings
                       yellow, explanations blue; clean and fclean say what they remove
  config.mk            names (ft_ssl, ft_ssl_bonus), flags, BONUS_FLAGS, directories,
                       Docker name
  libs.mk              libft path, and the files its rule depends on
  sources.mk           source list per folder (one list, used by both builds)
include/
  ft_ssl_md5.h         prototypes, under the name of the file that defines them
  core/                libs.h (system headers and libft), defines.h (BONUS_MODE and every
                       constant, under the names of the standards), enums.h, structs.h
libs/                libft (git submodule, see above)
sources/
  main.c               main() and nothing else: the usage (or, in the bonus, the command
                       reader) without a command, else the command
  utils/               what every command uses
                       dispatch.c  the command table, its lookup, the list of commands
                       buffer.c    a growing buffer, reading a file or the standard input
                       output.c    writing the results (and saying when it fails),
                                   error messages
  digest/              what every digest command shares (md5, sha256, whirlpool)
                       command.c   the arguments in order: the flags, -s, the files, and
                                   the standard input when nothing else was given
                       sources.c   the standard input read once, a file, hash and print
                       print.c     the four formats of a line, and the digest in hex
  hash/                md5 and sha256
                       md5.c            md5 from the message to the digest, and its command
                       md5_rounds.c     T, the shifts and the word of each step; one block
                       sha256.c         sha256 from the message to the digest, and its command
                       sha256_rounds.c  K, the message schedule, the 64 rounds; one block
                       padding.c        the padding md5 and sha256 share
  bonus/               the bonus (make bonus, -D BONUS_MODE=1), named *_bonus.c:
                         stdin_bonus.c             the command reader: a line, exit and quit
                         tokenize_bonus.c          a line into words, with quotes and \
                         tokens_bonus.c            the word being built, the array of words
                         whirlpool_bonus.c         whirlpool from the message to the digest,
                                                   its padding, and its command
                         whirlpool_block_bonus.c   one block (Miyaguchi-Preneel)
                         whirlpool_tables_bonus.c  the S-box, the tables and one round
Dockerfile           Linux test image: builds both programs, runs the tests/ scripts
tests/               setup.sh      helpers (sourced): checks, a stdin that stays open,
                                   valgrind
                     mandatory.sh  checks of ./ft_ssl, against md5sum and sha256sum
                     bonus.sh      checks of ./ft_ssl_bonus, against openssl whirlpool
```

No file holds more than five functions (the Norm's limit), and the file and folder names
live in a single place (`mk/sources.mk`). Both programs come from the same files and the
same `main()`: `main.c` is the only file at the root of `sources/`, every folder holds
files only. Each file keeps its helpers `static`: a function is declared in
`ft_ssl_md5.h` only when another file calls it.

## Technical choices

**A command table, not a forest of if/else.** The subject asks for it, and the next
ft_ssl projects add standard and cipher commands. Each command is a row of `t_command`:
its name, its family (`COMMAND_GROUP_MESSAGE_DIGEST`, and `STANDARD` and `CIPHER` for
later) and the function that runs it. The lookup and the error message read the same
table (`utils/dispatch.c`). Adding sha512, for example, is a `hash/sha512.c` with its
`run_sha512()` and one row:

```c
{"sha512", COMMAND_GROUP_MESSAGE_DIGEST, run_sha512},
```

**One pipeline for every digest.** A hash is only a `t_digest_algorithm`: its name, the
name it prints (`MD5`), its digest size and a `compute()` function, all static in its
file. Everything else (the flags, reading the standard input once, `-s`, files, the four
output formats, the errors) is in `digest/` and is the same for md5, sha256 and
whirlpool, which is what the subject asks for sha256 ("exactly the same as the command
above").

**The constants have the names of the standards.** Every size and constant is defined
in `include/core/defines.h`, and every set of values is an enum in
`include/core/enums.h`. The algorithms use the names their specifications give:
`MD5_INIT_A` to `MD5_INIT_D` and the shifts `MD5_S11` to `MD5_S44` of RFC 1321,
`SHA256_H0` to `SHA256_H7` of FIPS 180-4, and Whirlpool's `cir(1, 1, 4, 1, 8, 5, 2, 9)`.
The word of the block that each MD5 step reads is the rule of the RFC (a first word and a
stride per round), not a table of 64 numbers. What stays a table is what the
specifications list as one, in the same hexadecimal: T for MD5, K for SHA-256, the S-box
of Whirlpool. norminette misreads a hexadecimal constant that starts with `0xB`, so those
are written `0x0B...`, which C reads as the same number.

**The hashes.** MD5 works on little-endian 32-bit words, SHA-256 on big-endian ones;
both pad the message the same way (`hash/padding.c`): `0x80`, zeros, then its length in
bits on 64 bits. Whirlpool builds its eight tables and ten round constants on its first
round, from the S-box and the circulant row in GF(2^8), then runs a Miyaguchi-Preneel
compression on 512-bit blocks, with a 256-bit length. Each input is read whole, then
hashed: simple, and fast enough (for 50 MB, without optimisation flags, about 1 s for
md5, 1.6 s for sha256 and 4.4 s for whirlpool), at the cost of holding the input and its
padded copy in memory.

**The bonus never touches the mandatory build.** `make bonus` compiles every file again
with `-D BONUS_MODE=1` into `objects_bonus/`, and links `ft_ssl_bonus`;
`include/core/defines.h` sets `BONUS_MODE` to 0 otherwise. The `*_bonus.c` files are
wrapped in `#if BONUS_MODE`, so they are empty in `ft_ssl`. `main()` starts the command
reader under `if (BONUS_MODE)`: the function is declared in both builds, defined only in
the bonus, and the compiler drops the call from the mandatory one. The command table
adds the whirlpool row under `#if BONUS_MODE`, and the headers keep the bonus functions,
structures, enums and constants under `# if BONUS_MODE`.

**Errors are never silent.** Every allocation, `open` and `read`, and every write of a
result, is checked (an error message that cannot be written is not reported again). A
file that fails is reported and the next ones are still hashed; running out of memory
or failing to write stops the command. Results go through `write_all()` rather than
libft's output functions, which return nothing.

**No global variable that changes, except Whirlpool's tables.** The tables of MD5,
SHA-256 and the commands are `static const`. Whirlpool's eight tables of 256 entries are
computed once, on the first round, into its `static` table, so that the S-box and the
circulant row are the only data written by hand.

## Resources

**The subject's topics**
- Cryptographic hash function - https://en.wikipedia.org/wiki/Cryptographic_hash_function
- RFC 1321, The MD5 Message-Digest Algorithm - https://www.rfc-editor.org/rfc/rfc1321
- FIPS 180-4, Secure Hash Standard (SHA-256) - https://csrc.nist.gov/pubs/fips/180-4/upd1/final
- The WHIRLPOOL Hashing Function, P. Barreto and V. Rijmen - https://web.archive.org/web/20171129084214/http://www.larc.usp.br/~pbarreto/WhirlpoolPage.html
- Merkle-Damgard construction - https://en.wikipedia.org/wiki/Merkle%E2%80%93Damg%C3%A5rd_construction
- `man md5sum`, `man sha256sum`, `man openssl-dgst`, `man 2 read`, `man 2 write`

**Tools**
- Norminette - https://github.com/42School/norminette
- Valgrind - https://valgrind.org/docs/manual/manual.html

**Use of AI.** An AI assistant (Claude) was used for review,it checked the finished library against the subject and the Norm, pointed out failures and explained why they did not match the subject.
