/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256_algo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:31 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/15 23:44:15 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <errno.h>
#include <stdlib.h>

void	sha256_process_block(t_sha256_ctx *ctx, const unsigned char *block);

static void	write_u32_be(unsigned char *dst, uint32_t value)
{
	dst[0] = (unsigned char)((value >> 24) & 0xff);
	dst[1] = (unsigned char)((value >> 16) & 0xff);
	dst[2] = (unsigned char)((value >> 8) & 0xff);
	dst[3] = (unsigned char)(value & 0xff);
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
		buffer[new_len + (7 - index)] = (unsigned char)(bit_len & 0xff);
		bit_len >>= 8;
		index++;
	}
	return (buffer);
}

static void	init_ctx(t_sha256_ctx *ctx)
{
	ctx->h[0] = 1779033703;
	ctx->h[1] = (uint32_t)-1150833019;
	ctx->h[2] = 1013904242;
	ctx->h[3] = (uint32_t)-1521486534;
	ctx->h[4] = 1359893119;
	ctx->h[5] = (uint32_t)-1694144372;
	ctx->h[6] = 528734635;
	ctx->h[7] = 1541459225;
}

static int	sha256_compute(const unsigned char *data, size_t len,
	unsigned char *digest)
{
	t_sha256_ctx	ctx;
	unsigned char	*buffer;
	size_t			padded_len;
	size_t			offset;
	int				index;

	init_ctx(&ctx);
	buffer = pad_message(data, len, &padded_len);
	if (buffer == NULL)
		return (0);
	offset = 0;
	while (offset < padded_len)
	{
		sha256_process_block(&ctx, buffer + offset);
		offset += 64;
	}
	free(buffer);
	index = -1;
	while (++index < 8)
		write_u32_be(digest + (index * 4), ctx.h[index]);
	return (1);
}

const t_digest_algorithm	*get_sha256_algorithm(void)
{
	static const t_digest_algorithm	algorithm = {
		"sha256",
		"SHA256",
		32,
		sha256_compute
	};

	return (&algorithm);
}
