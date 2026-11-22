# Linux test image for ft_ssl: builds ft_ssl and ft_ssl_bonus, then runs the checks:
# tests/mandatory.sh, then tests/bonus.sh, both using tests/setup.sh.
#   - every example of the subject must give exactly its output;
#   - md5 and sha256 must match md5sum and sha256sum, whirlpool must match
#     openssl, on every padding case and on large files;
#   - errors (missing, unreadable files, a closed stdin, a full disk) must be
#     reported, never read the standard input instead, and never leak;
#   - the bonus must read its commands from the standard input until exit,
#     quit or the end of the input.
# The exit status is the total number of failed checks. The libft submodule in
# libs/ must be checked out first: it is copied into the image as it is. The
# docker-* rules of the Makefile do this for you.
#
#   make docker-mandatory                              # mandatory part only
#   make docker-bonus                                  # bonus part only
#   make docker-shell                                  # bash inside the image
#   make docker-build && docker run --rm ft_ssl-test   # mandatory, then bonus
#   make docker-clean                                  # removes the image
FROM debian:trixie

# openssl-provider-legacy: openssl 3 only computes whirlpool with it.
RUN apt-get update \
	&& DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
		build-essential valgrind openssl openssl-provider-legacy \
	&& rm -rf /var/lib/apt/lists/*

# Not root: a file without read permission must stay unreadable to the checks.
RUN useradd --create-home tester && mkdir /work && chown tester:tester /work
WORKDIR /work
COPY --chown=tester:tester . .
USER tester
RUN make && make bonus

CMD ["bash", "-c", "bash tests/mandatory.sh; m=$?; bash tests/bonus.sh; exit $((m + $?))"]
