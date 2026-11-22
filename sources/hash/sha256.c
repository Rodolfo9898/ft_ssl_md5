/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:31 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 16:57:15 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/* The digest: each word of the state, most significant byte first */
static void	write_state_be(unsigned char *digest, const uint32_t *state)
{
	int			word;
	int			byte;
	uint32_t	value;

	word = -1;
	while (++word < SHA256_STATE_WORDS)
	{
		value = state[word];
		byte = WORD_SIZE;
		while (byte-- > 0)
		{
			digest[word * WORD_SIZE + byte] = value & BYTE_MASK;
			value >>= BYTE_BITS;
		}
	}
}

/* SHA-256 (FIPS 180-4): 0 when the padded copy cannot be allocated */
static int	sha256_compute(const unsigned char *data, size_t len,
	unsigned char *digest)
{
	static const uint32_t	init[SHA256_STATE_WORDS] = {SHA256_H0, SHA256_H1,
		SHA256_H2, SHA256_H3, SHA256_H4, SHA256_H5, SHA256_H6, SHA256_H7};
	t_sha256_ctx			ctx;
	unsigned char			*buffer;
	size_t					padded_len;
	size_t					offset;

	ft_memcpy(ctx.h, init, sizeof(init));
	buffer = pad_message(data, len, &padded_len, ORDER_BIG_ENDIAN);
	if (buffer == NULL)
		return (0);
	offset = 0;
	while (offset < padded_len)
	{
		sha256_process_block(&ctx, buffer + offset);
		offset += BLOCK_SIZE;
	}
	free(buffer);
	write_state_be(digest, ctx.h);
	return (1);
}

/* The sha256 command: the digest pipeline of digest/, with SHA-256 */
int	run_sha256(int argc, char **argv)
{
	static const t_digest_algorithm	sha256 = {"sha256", "SHA256",
		SHA256_DIGEST_SIZE, sha256_compute};

	return (run_digest_command(&sha256, argc, argv));
}
