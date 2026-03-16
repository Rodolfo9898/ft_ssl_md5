/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 21:55:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/16 21:37:22 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "libs.h"
# include "enum.h"

typedef struct s_command
{
	const char		*name;
	t_command_group	group;
	int				(*handler)(const struct s_command *c, int ac, char **av);
	const void		*data;
}	t_command;

typedef struct s_buffer
{
	unsigned char	*data;
	size_t			len;
	size_t			capacity;
}	t_buffer;

typedef struct s_digest_algorithm
{
	const char	*name;
	const char	*display_name;
	size_t		digest_size;
	int			(*compute)(const unsigned char *data, size_t len,
			unsigned char *digest);
}	t_digest_algorithm;

typedef struct s_digest_options
{
	int	quiet;
	int	reverse;
}	t_digest_options;

typedef struct s_md5_ctx
{
	uint32_t	a;
	uint32_t	b;
	uint32_t	c;
	uint32_t	d;
}	t_md5_ctx;

typedef struct s_sha256_ctx
{
	uint32_t	h[8];
}	t_sha256_ctx;

typedef struct s_md5_tables
{
	uint32_t	s[64];
	uint32_t	k[64];
}	t_md5_tables;

typedef struct s_sha256_tables
{
	uint32_t	k[64];
}	t_sha256_tables;

typedef struct s_whirlpool_tables
{
	uint16_t	sbox_words[128];
	uint64_t	c[8][256];
	uint64_t	rc[11];
	int			initialized;
}	t_whirlpool_tables;

typedef struct s_digest_runtime
{
	const t_digest_algorithm	*algorithm;
	t_digest_options			options;
	int							parsed_operand;
	int							processed_any;
	int							had_error;
	int							stdin_loaded;
	t_buffer					stdin_buffer;
}	t_digest_runtime;

typedef struct s_digest_print
{
	t_digest_source_kind	source_kind;
	const char				*label;
	size_t					label_len;
	const unsigned char		*digest;
}	t_digest_print;

typedef struct s_split_state
{
	char	**argv;
	char	*token;
	size_t	index;
	size_t	len;
	size_t	capacity;
	int		argc;
	int		argv_capacity;
	char	quote;
}	t_split_state;

#endif
