/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_md5.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 16:57:02 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 18:29:45 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_MD5_H
# define FT_SSL_MD5_H

# include "core/structs.h"

/*
** Only what more than one file uses: everything else is static in its file.
** The string and memory helpers, and the error output, come from libft.
*/

/* From utils/buffer.c */
int				buffer_append(t_buffer *buffer, const unsigned char *data,
					size_t len);
void			buffer_free(t_buffer *buffer);
int				read_stream(int fd, t_buffer *buffer);
int				read_file(const char *path, t_buffer *buffer);

/* From utils/output.c */
int				write_all(int fd, const void *data, size_t len);
int				write_str(int fd, const char *str);
void			print_system_error(char *command, char *target);

/* From bits/logic.c: the only bitwise operators, see there */
uint32_t		choose(uint32_t x, uint32_t y, uint32_t z);
uint32_t		majority(uint32_t x, uint32_t y, uint32_t z);
uint32_t		parity(uint32_t x, uint32_t y, uint32_t z);
uint32_t		md5_i(uint32_t x, uint32_t y, uint32_t z);
uint64_t		xor64(uint64_t a, uint64_t b);

/* From bits/rotate.c: shifts and rotations, as products and divisions */
uint64_t		power_of_two(int n);
uint32_t		rotate_left32(uint32_t value, int n);
uint32_t		rotate_right32(uint32_t value, int n);
uint32_t		shift_right32(uint32_t value, int n);
uint64_t		rotate_right64(uint64_t value, int n);

/* From utils/dispatch.c */
int				execute_command_argv(int argc, char **argv);

/* From digest/command.c */
int				run_digest_command(const t_digest_algorithm *algorithm,
					int argc, char **argv);

/* From digest/sources.c */
int				digest_and_print(t_digest_runtime *runtime,
					const t_digest_print *print, const t_buffer *buffer);
int				process_digest_file(t_digest_runtime *runtime, char *path);
int				process_digest_stdin(t_digest_runtime *runtime,
					t_digest_source_kind kind);

/* From digest/print.c */
int				format_digest_output(const t_digest_algorithm *algorithm,
					const t_digest_options *options,
					const t_digest_print *print);

/* From hash/padding.c */
unsigned char	*pad_message(const unsigned char *data, size_t len,
					size_t *padded_len, t_byte_order order);

/* From hash/md5.c and hash/md5_rounds.c */
int				run_md5(int argc, char **argv);
void			md5_process_block(t_md5_ctx *ctx, const unsigned char *block);

/* From hash/sha256.c and hash/sha256_rounds.c */
int				run_sha256(int argc, char **argv);
void			sha256_process_block(t_sha256_ctx *ctx,
					const unsigned char *block);

/*
** Bonus entry point: declared in both builds because main() calls it under
** 'if (BONUS_MODE)', but only defined with -D BONUS_MODE=1.
*/
/* From bonus/stdin_bonus.c */
int				run_stdin_commands(void);

/* Hidden from the mandatory build: only with -D BONUS_MODE=1 */
# if BONUS_MODE

/* From bonus/tokenize_bonus.c */
int				tokenize_line(const char *line, int *argc_out,
					char ***argv_out);

/* From bonus/tokens_bonus.c */
int				split_append_char(t_split_state *state, char c);
int				split_append_token(t_split_state *state);
void			free_argv_tokens(char **argv, int argc);

/* From bonus/whirlpool_bonus.c */
int				run_whirlpool(int argc, char **argv);

/* From bonus/whirlpool_build_bonus.c */
void			whirlpool_build(t_whirlpool_tables *t);

/* From bonus/whirlpool_tables_bonus.c */
void			whirlpool_round(uint64_t *key, uint64_t *state, int round);

/* From bonus/whirlpool_block_bonus.c */
void			whirlpool_process_buffer(uint64_t *hash,
					const unsigned char *buffer);

# endif

#endif
