/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_algo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:22 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/15 23:34:48 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <errno.h>
#include <stdlib.h>

void	md5_process_block(t_md5_ctx *ctx, const unsigned char *block);

static void	write_u32_le(unsigned char *dst, uint32_t value)
{
	dst[0] = (unsigned char)(value & 0xff);
	dst[1] = (unsigned char)((value >> 8) & 0xff);
	dst[2] = (unsigned char)((value >> 16) & 0xff);
	dst[3] = (unsigned char)((value >> 24) & 0xff);
}

static unsigned char	*pad_message(const unsigned char *data, size_t len,
	size_t *padded_len)
{
	unsigned char	*buffer;
	uint64_t		bit_len;
	size_t			new_len;
	size_t			index;

	new_len = len + 1;
	while ((new_len % 64) != 56)
		new_len++;
	*padded_len = new_len + 8;
	buffer = (unsigned char *)malloc(*padded_len);
	if (buffer == NULL)
		return (errno = ENOMEM, NULL);
	ft_memcpy(buffer, data, len);
	buffer[len] = 0x80;
	if (new_len > len + 1)
		ft_memset(buffer + len + 1, 0, new_len - len - 1);
	bit_len = (uint64_t)len * 8;
	index = 0;
	while (index < 8)
	{
		buffer[new_len + index] = (unsigned char)(bit_len >> (index * 8));
		index++;
	}
	return (buffer);
}

static int	md5_compute(const unsigned char *data, size_t len,
	unsigned char *digest)
{
	t_md5_ctx		ctx;
	unsigned char	*buffer;
	size_t			padded_len;
	size_t			offset;

	ctx.a = 0x67452301;
	ctx.b = 0xefcdab89;
	ctx.c = 0x98badcfe;
	ctx.d = 0x10325476;
	buffer = pad_message(data, len, &padded_len);
	if (buffer == NULL)
		return (0);
	offset = 0;
	while (offset < padded_len)
	{
		md5_process_block(&ctx, buffer + offset);
		offset += 64;
	}
	free(buffer);
	write_u32_le(digest, ctx.a);
	write_u32_le(digest + 4, ctx.b);
	write_u32_le(digest + 8, ctx.c);
	write_u32_le(digest + 12, ctx.d);
	return (1);
}

const t_digest_algorithm	*get_md5_algorithm(void)
{
	static const t_digest_algorithm	algorithm = {
		"md5",
		"MD5",
		16,
		md5_compute
	};

	return (&algorithm);
}
