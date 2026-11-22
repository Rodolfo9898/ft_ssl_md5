/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enums.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 21:52:58 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 16:10:55 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUMS_H
# define ENUMS_H

# include "defines.h"

/* The families of commands: the digests now, the others in later projects */
typedef enum e_command_group
{
	COMMAND_GROUP_STANDARD,
	COMMAND_GROUP_MESSAGE_DIGEST,
	COMMAND_GROUP_CIPHER
}	t_command_group;

/* What a digest line shows: the standard input, -p, -s or a file */
typedef enum e_digest_source_kind
{
	DIGEST_SOURCE_STDIN_DEFAULT,
	DIGEST_SOURCE_STDIN_P,
	DIGEST_SOURCE_STRING,
	DIGEST_SOURCE_FILE
}	t_digest_source_kind;

/* How the length of a padded message is written: MD5, then SHA-256 */
typedef enum e_byte_order
{
	ORDER_LITTLE_ENDIAN,
	ORDER_BIG_ENDIAN
}	t_byte_order;

/* The four words of the MD5 state, A to D */
typedef enum e_md5_word
{
	MD5_A,
	MD5_B,
	MD5_C,
	MD5_D
}	t_md5_word;

/* The four rounds of MD5, each with its function: F, G, H and I */
typedef enum e_md5_round
{
	MD5_ROUND_F,
	MD5_ROUND_G,
	MD5_ROUND_H,
	MD5_ROUND_I
}	t_md5_round;

/* The eight working variables of SHA-256, a to h */
typedef enum e_sha256_var
{
	SHA_A,
	SHA_B,
	SHA_C,
	SHA_D,
	SHA_E,
	SHA_F,
	SHA_G,
	SHA_H
}	t_sha256_var;

/*
** Only with -D BONUS_MODE=1. What the command reader does next, and the two
** fields of the last bytes of a Whirlpool message: where they start, how many.
*/
# if BONUS_MODE

typedef enum e_reader_status
{
	READER_ERROR = -1,
	READER_STOP,
	READER_CONTINUE
}	t_reader_status;

typedef enum e_tail
{
	TAIL_START,
	TAIL_LEN,
	TAIL_FIELDS
}	t_tail;

# endif

#endif
