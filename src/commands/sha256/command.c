/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:35 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/15 23:27:36 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static const uint32_t	g_sha256_k[64] = {
	1116352408, 1899447441, 3049323471, 3921009573,
	961987163, 1508970993, 2453635748, 2870763221,
	3624381080, 310598401, 607225278, 1426881987,
	1925078388, 2162078206, 2614888103, 3248222580,
	3835390401, 4022224774, 264347078, 604807628,
	770255983, 1249150122, 1555081692, 1996064986,
	2554220882, 2821834349, 2952996808, 3210313671,
	3336571891, 3584528711, 113926993, 338241895,
	666307205, 773529912, 1294757372, 1396182291,
	1695183700, 1986661051, 2177026350, 2456956037,
	2730485921, 2820302411, 3259730800, 3345764771,
	3516065817, 3600352804, 4094571909, 275423344,
	430227734, 506948616, 659060556, 883997877,
	958139571, 1322822218, 1537002063, 1747873779,
	1955562222, 2024104815, 2227730452, 2361852424,
	2428436474, 2756734187, 3204031479, 3329325298
};

static uint32_t	right_rotate(uint32_t value, uint32_t shift)
{
	return ((value >> shift) | (value << (32 - shift)));
}

static uint32_t	read_u32_be(const unsigned char *src)
{
	return (((uint32_t)src[0] << 24) | ((uint32_t)src[1] << 16)
		| ((uint32_t)src[2] << 8) | (uint32_t)src[3]);
}

static void	build_schedule(uint32_t words[64], const unsigned char *block)
{
	int			index;
	uint32_t	tmp[2];

	index = -1;
	while (++index < 16)
		words[index] = read_u32_be(block + (index * 4));
	while (index < 64)
	{
		tmp[0] = right_rotate(words[index - 15], 7);
		tmp[0] ^= right_rotate(words[index - 15], 18);
		tmp[0] ^= words[index - 15] >> 3;
		tmp[1] = right_rotate(words[index - 2], 17);
		tmp[1] ^= right_rotate(words[index - 2], 19);
		tmp[1] ^= words[index - 2] >> 10;
		words[index] = words[index - 16] + tmp[0];
		words[index] += words[index - 7] + tmp[1];
		index++;
	}
}

static void	round_step(uint32_t state[8], uint32_t word, int index)
{
	uint32_t	tmp[2];

	tmp[0] = state[7];
	tmp[0] += right_rotate(state[4], 6) ^ right_rotate(state[4], 11)
		^ right_rotate(state[4], 25);
	tmp[0] += (state[4] & state[5]) ^ ((~state[4]) & state[6]);
	tmp[0] += g_sha256_k[index] + word;
	tmp[1] = right_rotate(state[0], 2) ^ right_rotate(state[0], 13);
	tmp[1] ^= right_rotate(state[0], 22);
	tmp[1] += (state[0] & state[1]) ^ (state[0] & state[2]);
	tmp[1] += state[1] & state[2];
	state[7] = state[6];
	state[6] = state[5];
	state[5] = state[4];
	state[4] = state[3] + tmp[0];
	state[3] = state[2];
	state[2] = state[1];
	state[1] = state[0];
	state[0] = tmp[0] + tmp[1];
}

void	sha256_process_block(t_sha256_ctx *ctx, const unsigned char *block)
{
	uint32_t	words[64];
	uint32_t	state[8];
	int			index;

	build_schedule(words, block);
	index = -1;
	while (++index < 8)
		state[index] = ctx->h[index];
	index = -1;
	while (++index < 64)
		round_step(state, words[index], index);
	index = -1;
	while (++index < 8)
		ctx->h[index] += state[index];
}
