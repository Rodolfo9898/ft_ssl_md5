/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:27 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/15 23:27:28 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static const uint32_t	g_md5_s[64] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

static const uint32_t	g_md5_k[64] = {
	0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE,
	0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501,
	0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE,
	0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821,
	0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA,
	0xD62F105D, 0x02441453, 0xD8A1E681, 0xE7D3FBC8,
	0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED,
	0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A,
	0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C,
	0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70,
	0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x04881D05,
	0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665,
	0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039,
	0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1,
	0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1,
	0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391
};

static uint32_t	left_rotate(uint32_t value, uint32_t shift)
{
	return ((value << shift) | (value >> (32 - shift)));
}

static uint32_t	read_u32_le(const unsigned char *src)
{
	return ((uint32_t)src[0] | ((uint32_t)src[1] << 8)
		| ((uint32_t)src[2] << 16) | ((uint32_t)src[3] << 24));
}

static void	round_step(int index, uint32_t state[4], const uint32_t words[16])
{
	uint32_t	f;
	uint32_t	g;

	if (index < 16)
		f = (state[1] & state[2]) | ((~state[1]) & state[3]);
	else if (index < 32)
		f = (state[3] & state[1]) | ((~state[3]) & state[2]);
	else if (index < 48)
		f = state[1] ^ state[2] ^ state[3];
	else
		f = state[2] ^ (state[1] | (~state[3]));
	g = index;
	if (index >= 16 && index < 32)
		g = (uint32_t)((5 * index + 1) % 16);
	else if (index >= 32 && index < 48)
		g = (uint32_t)((3 * index + 5) % 16);
	else if (index >= 48)
		g = (uint32_t)((7 * index) % 16);
	f += state[0] + g_md5_k[index] + words[g];
	state[0] = state[3];
	state[3] = state[2];
	state[2] = state[1];
	state[1] += left_rotate(f, g_md5_s[index]);
}

void	md5_process_block(t_md5_ctx *ctx, const unsigned char *block)
{
	uint32_t	words[16];
	uint32_t	state[4];
	int			index;

	index = -1;
	while (++index < 16)
		words[index] = read_u32_le(block + (index * 4));
	state[0] = ctx->a;
	state[1] = ctx->b;
	state[2] = ctx->c;
	state[3] = ctx->d;
	index = -1;
	while (++index < 64)
		round_step(index, state, words);
	ctx->a += state[0];
	ctx->b += state[1];
	ctx->c += state[2];
	ctx->d += state[3];
}
