/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_rounds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:27 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 18:29:45 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/*
** RFC 1321, 3.4, see include/core/defines.h: T, the constant of each step
** (t[0] is T1), then the shifts of each round, the first word each round
** reads, and how it moves to the next one.
*/
static const t_md5_tables	g_md5_tables = {
{
	MD5_T1, MD5_T2, MD5_T3, MD5_T4,
	MD5_T5, MD5_T6, MD5_T7, MD5_T8,
	MD5_T9, MD5_T10, MD5_T11, MD5_T12,
	MD5_T13, MD5_T14, MD5_T15, MD5_T16,
	MD5_T17, MD5_T18, MD5_T19, MD5_T20,
	MD5_T21, MD5_T22, MD5_T23, MD5_T24,
	MD5_T25, MD5_T26, MD5_T27, MD5_T28,
	MD5_T29, MD5_T30, MD5_T31, MD5_T32,
	MD5_T33, MD5_T34, MD5_T35, MD5_T36,
	MD5_T37, MD5_T38, MD5_T39, MD5_T40,
	MD5_T41, MD5_T42, MD5_T43, MD5_T44,
	MD5_T45, MD5_T46, MD5_T47, MD5_T48,
	MD5_T49, MD5_T50, MD5_T51, MD5_T52,
	MD5_T53, MD5_T54, MD5_T55, MD5_T56,
	MD5_T57, MD5_T58, MD5_T59, MD5_T60,
	MD5_T61, MD5_T62, MD5_T63, MD5_T64
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

/* A 32-bit word, least significant byte first */
static uint32_t	read_u32_le(const unsigned char *src)
{
	uint32_t	value;
	int			index;

	value = 0;
	index = WORD_SIZE - 1;
	while (index >= 0)
	{
		value = value * BYTE_RANGE + src[index];
		index--;
	}
	return (value);
}

/*
** The function of each round (RFC 1321, 3.4): F chooses between C and D with
** B, G between B and C with D, H is the parity of B, C and D, and I is
** md5_i (see bits/logic.c).
*/
static uint32_t	round_function(t_md5_round round, const uint32_t *state)
{
	if (round == MD5_ROUND_F)
		return (choose(state[MD5_B], state[MD5_C], state[MD5_D]));
	if (round == MD5_ROUND_G)
		return (choose(state[MD5_D], state[MD5_B], state[MD5_C]));
	if (round == MD5_ROUND_H)
		return (parity(state[MD5_B], state[MD5_C], state[MD5_D]));
	return (md5_i(state[MD5_B], state[MD5_C], state[MD5_D]));
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
	state[MD5_B] += rotate_left32(f,
			g_md5_tables.shift[round][place % MD5_SHIFTS_PER_ROUND]);
}

/* One block of 64 bytes: 64 steps, in four rounds of 16 */
void	md5_process_block(t_md5_ctx *ctx, const unsigned char *block)
{
	uint32_t	words[MD5_BLOCK_WORDS];
	uint32_t	state[MD5_STATE_WORDS];
	int			index;

	index = 0;
	while (index < MD5_BLOCK_WORDS)
	{
		words[index] = read_u32_le(block + index * WORD_SIZE);
		index++;
	}
	ft_memcpy(state, ctx->h, sizeof(state));
	index = 0;
	while (index < MD5_STEPS)
	{
		round_step(index, state, words);
		index++;
	}
	index = 0;
	while (index < MD5_STATE_WORDS)
	{
		ctx->h[index] += state[index];
		index++;
	}
}
