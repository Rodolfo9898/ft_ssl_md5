/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 20:05:21 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/16 21:52:24 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include "core/libs.h"
# include "core/enum.h"
# include "core/structs.h"

int							ft_is_streq(const char *a, const char *b);
size_t						ft_strlen(const char *str);
char						*ft_strdup(const char *src);
int							ft_strcmp(const char *a, const char *b);
void						ft_memcpy(void *dst, const void *src, size_t len);
void						ft_memset(void *dst, int c, size_t len);

int							buffer_append(t_buffer *buffer,
								const unsigned char *data, size_t len);
void						buffer_free(t_buffer *buffer);
int							read_stream(int fd, t_buffer *buffer);
int							read_file(const char *path, t_buffer *buffer);
int							write_all(int fd, const void *data, size_t len);

void						ft_putstr_fd(int fd, const char *str);
void						ft_putendl_fd(int fd, const char *str);
void						ft_puthex_fd(int fd, const unsigned char *data,
								size_t len);
void						print_invalid_command(const char *name);
void						print_system_error(const char *command,
								const char *target);
void						print_command_list(void);
void						print_usage(void);

const t_digest_algorithm	*get_md5_algorithm(void);
const t_digest_algorithm	*get_sha256_algorithm(void);
const t_digest_algorithm	*get_whirlpool_algorithm(void);

const t_command				*find_command(const char *name);
const t_command				*get_command_table(size_t *count);
int							execute_command_argv(int argc, char **argv);

int							run_help_command(const t_command *command,
								int argc, char **argv);
int							run_digest_command(const t_command *command,
								int argc, char **argv);
int							run_stdin_commands(void);
int							tokenize_line(const char *line, int *argc_out,
								char ***argv_out);
void						free_argv_tokens(char **argv, int argc);
int							split_append_char(t_split_state *state, char c);
int							split_append_token(t_split_state *state);

void						md5_process_block(t_md5_ctx *c,
								const unsigned char *block);
void						sha256_process_block(t_sha256_ctx *ctx,
								const unsigned char *block);
void						whirlpool_init_tables(void);
void						whirlpool_process_buffer(uint64_t hash[8],
								const unsigned char *buffer);
uint64_t					whirlpool_transform(const uint64_t src[8],
								int index);
uint64_t					whirlpool_round_constant(int round);
void						whirlpool_init_round_key(int round,
								uint64_t (*c)[256], uint64_t *rc);

int							process_digest_stdin(t_digest_runtime *run, int m);
int							handle_digest_arg(t_digest_runtime *run, int argc,
								char **argv, int *index);
int							process_digest_string(t_digest_runtime *run,
								const char *v);
void						process_digest_file(t_digest_runtime *run,
								const char *dir);
void						format_digest_output(const t_digest_algorithm *algo,
								const t_digest_options *options,
								const t_digest_print *print);

#endif
