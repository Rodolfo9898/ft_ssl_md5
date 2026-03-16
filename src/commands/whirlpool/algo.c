/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:39 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/16 21:43:09 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static void	build_length(unsigned char bit_length[32], size_t len)
{
	uint64_t	bits;
	int			index;

	ft_memset(bit_length, 0, 32);
	bits = (uint64_t)len * 8;
	index = 0;
	while (index < 8)
	{
		bit_length[31 - index] = (unsigned char)(bits & 0xff);
		bits >>= 8;
		index++;
	}
}

static void	process_tail(uint64_t hash[8], unsigned char block[64],
	const unsigned char *data, size_t tail[2])
{
	unsigned char	bit_length[32];

	ft_memset(block, 0, 64);
	if (tail[1] > 0)
		ft_memcpy(block, data + tail[0], tail[1]);
	block[tail[1]] |= 0x80;
	if (tail[1] + 1 > 32)
	{
		whirlpool_process_buffer(hash, block);
		ft_memset(block, 0, 64);
	}
	build_length(bit_length, tail[0] + tail[1]);
	ft_memcpy(block + 32, bit_length, 32);
	whirlpool_process_buffer(hash, block);
}

static void	write_digest(unsigned char *digest, const uint64_t hash[8])
{
	int			index;
	int			b;
	uint64_t	value;

	index = -1;
	while (++index < 8)
	{
		value = hash[index];
		b = -1;
		while (++b < 8)
			digest[index * 8 + b] = (unsigned char)(value >> (56 - b * 8));
	}
}

static int	whirlpool_compute(const unsigned char *data, size_t len,
	unsigned char *digest)
{
	uint64_t		hash[8];
	unsigned char	block[64];
	size_t			tail[2];

	whirlpool_init_tables();
	ft_memset(hash, 0, sizeof(hash));
	tail[0] = 0;
	while (tail[0] + 64 <= len)
	{
		whirlpool_process_buffer(hash, data + tail[0]);
		tail[0] += 64;
	}
	tail[1] = len - tail[0];
	process_tail(hash, block, data, tail);
	write_digest(digest, hash);
	return (1);
}

const t_digest_algorithm	*get_whirlpool_algorithm(void)
{
	static const t_digest_algorithm	algorithm = {
		"whirlpool",
		"WHIRLPOOL",
		64,
		whirlpool_compute
	};

	return (&algorithm);
}
