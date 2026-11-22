/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_rounds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:27 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 16:57:15 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/*
** RFC 1321, 3.4: T, the constant of each step (the integer part of
** 4294967296 * abs(sin(i)) for step i, from 1), then the shifts of each
** round, the first word each round reads, and how it moves to the next one.
*/
static const t_md5_tables	g_md5_tables = {
{
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
},
{
{MD5_S11, MD5_S12, MD5_S13, MD5_S14},
{MD5_S21, MD5_S22, MD5_S23, MD5_S24},
{MD5_S31, MD5_S32, MD5_S33, MD5_S34},
{MD5_S41, MD5_S42, MD5_S43, MD5_S44}
},
{MD5_F_FIRST_WORD, MD5_G_FIRST_WORD, MD5_H_FIRST_WORD, MD5_I_FIRST_WORD},
{MD5_F_WORD_STRIDE, MD5_G_WORD_STRIDE, MD5_H_WORD_STRIDE, MD5_I_WORD_STRIDE}
};

static uint32_t	left_rotate(uint32_t value, uint32_t shift)
{
	return ((value << shift) | (value >> (WORD_BITS - shift)));
}

/* A 32-bit word, least significant byte first */
static uint32_t	read_u32_le(const unsigned char *src)
{
	uint32_t	value;
	int			index;

	value = 0;
	index = WORD_SIZE;
	while (index-- > 0)
		value = (value << BYTE_BITS) | src[index];
	return (value);
}

/* The function of each round: F, G, H or I */
static uint32_t	round_function(t_md5_round round, const uint32_t *state)
{
	if (round == MD5_ROUND_F)
		return ((state[MD5_B] & state[MD5_C])
			| (~state[MD5_B] & state[MD5_D]));
	if (round == MD5_ROUND_G)
		return ((state[MD5_D] & state[MD5_B])
			| (~state[MD5_D] & state[MD5_C]));
	if (round == MD5_ROUND_H)
		return (state[MD5_B] ^ state[MD5_C] ^ state[MD5_D]);
	return (state[MD5_C] ^ (state[MD5_B] | ~state[MD5_D]));
}

/* One step: its round, its place in the round, the word it reads */
static void	round_step(int step, uint32_t *state, const uint32_t *words)
{
	t_md5_round	round;
	int			place;
	uint32_t	word;
	uint32_t	f;

	round = (t_md5_round)(step / MD5_ROUND_STEPS);
	place = step % MD5_ROUND_STEPS;
	word = (g_md5_tables.first_word[round]
			+ g_md5_tables.word_stride[round] * place) % MD5_BLOCK_WORDS;
	f = round_function(round, state) + state[MD5_A] + g_md5_tables.t[step]
		+ words[word];
	state[MD5_A] = state[MD5_D];
	state[MD5_D] = state[MD5_C];
	state[MD5_C] = state[MD5_B];
	state[MD5_B] += left_rotate(f,
			g_md5_tables.shift[round][place % MD5_SHIFTS_PER_ROUND]);
}

/* One block of 64 bytes: 64 steps, in four rounds of 16 */
void	md5_process_block(t_md5_ctx *ctx, const unsigned char *block)
{
	uint32_t	words[MD5_BLOCK_WORDS];
	uint32_t	state[MD5_STATE_WORDS];
	int			index;

	index = -1;
	while (++index < MD5_BLOCK_WORDS)
		words[index] = read_u32_le(block + index * WORD_SIZE);
	ft_memcpy(state, ctx->h, sizeof(state));
	index = -1;
	while (++index < MD5_STEPS)
		round_step(index, state, words);
	index = -1;
	while (++index < MD5_STATE_WORDS)
		ctx->h[index] += state[index];
}
