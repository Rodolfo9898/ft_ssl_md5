/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 21:52:58 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 18:29:45 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "defines.h"
# include "libs.h"
# include "enums.h"

/*
** One row of the command table: its name, its family, and the function that
** runs it with the arguments after the name.
*/
typedef struct s_command
{
	char			*name;
	t_command_group	group;
	int				(*handler)(int argc, char **argv);
}	t_command;

typedef struct s_buffer
{
	unsigned char	*data;
	size_t			len;
	size_t			capacity;
}	t_buffer;

/* A hash: its command, the name it prints, its size, and the function */
typedef struct s_digest_algorithm
{
	char	*name;
	char	*display_name;
	size_t	digest_size;
	int		(*compute)(const unsigned char *data, size_t len,
			unsigned char *digest);
}	t_digest_algorithm;

typedef struct s_digest_options
{
	int	quiet;
	int	reverse;
}	t_digest_options;

typedef struct s_md5_ctx
{
	uint32_t	h[MD5_STATE_WORDS];
}	t_md5_ctx;

typedef struct s_sha256_ctx
{
	uint32_t	h[SHA256_STATE_WORDS];
}	t_sha256_ctx;

/*
** RFC 1321: the constant T of each step, then for each round its shifts, its
** first word and its word stride.
*/
typedef struct s_md5_tables
{
	uint32_t	t[MD5_STEPS];
	uint32_t	shift[MD5_ROUNDS][MD5_SHIFTS_PER_ROUND];
	uint32_t	first_word[MD5_ROUNDS];
	uint32_t	word_stride[MD5_ROUNDS];
}	t_md5_tables;

typedef struct s_sha256_tables
{
	uint32_t	k[SHA256_ROUNDS];
}	t_sha256_tables;

/* One digest command while it runs: its flags, and what it has done */
typedef struct s_digest_runtime
{
	const t_digest_algorithm	*algorithm;
	t_digest_options			options;
	int							parsed_operand;
	int							source_given;
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

/* Only with -D BONUS_MODE=1: whirlpool, and the words of the command reader */
# if BONUS_MODE

/*
** The S-box, the first row of the circulant matrix C, and what they give: the
** eight tables, and the constant of each round (rc[0] is unused).
*/
typedef struct s_whirlpool_tables
{
	uint8_t		sbox[SBOX_SIZE];
	uint8_t		circulant[WHIRLPOOL_WORD_SIZE];
	uint64_t	c[WHIRLPOOL_TABLES][SBOX_SIZE];
	uint64_t	rc[WHIRLPOOL_ROUNDS + 1];
	int			initialized;
}	t_whirlpool_tables;

/* The eight bytes of each of the eight words of a Whirlpool state */
typedef struct s_whirlpool_bytes
{
	unsigned char	byte[WHIRLPOOL_WORDS][WHIRLPOOL_WORD_SIZE];
}	t_whirlpool_bytes;

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

# endif

#endif
