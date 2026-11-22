/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:22 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 16:57:15 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/* The digest: each word of the state, least significant byte first */
static void	write_state_le(unsigned char *digest, const uint32_t *state)
{
	int			word;
	int			byte;
	uint32_t	value;

	word = -1;
	while (++word < MD5_STATE_WORDS)
	{
		value = state[word];
		byte = -1;
		while (++byte < WORD_SIZE)
		{
			digest[word * WORD_SIZE + byte] = value & BYTE_MASK;
			value >>= BYTE_BITS;
		}
	}
}

/* MD5 (RFC 1321): 0 when the padded copy cannot be allocated */
static int	md5_compute(const unsigned char *data, size_t len,
	unsigned char *digest)
{
	t_md5_ctx		ctx;
	unsigned char	*buffer;
	size_t			padded_len;
	size_t			offset;

	ctx.h[MD5_A] = MD5_INIT_A;
	ctx.h[MD5_B] = MD5_INIT_B;
	ctx.h[MD5_C] = MD5_INIT_C;
	ctx.h[MD5_D] = MD5_INIT_D;
	buffer = pad_message(data, len, &padded_len, ORDER_LITTLE_ENDIAN);
	if (buffer == NULL)
		return (0);
	offset = 0;
	while (offset < padded_len)
	{
		md5_process_block(&ctx, buffer + offset);
		offset += BLOCK_SIZE;
	}
	free(buffer);
	write_state_le(digest, ctx.h);
	return (1);
}

/* The md5 command: the digest pipeline of digest/, with MD5 */
int	run_md5(int argc, char **argv)
{
	static const t_digest_algorithm	md5 = {"md5", "MD5", MD5_DIGEST_SIZE,
		md5_compute};

	return (run_digest_command(&md5, argc, argv));
}
