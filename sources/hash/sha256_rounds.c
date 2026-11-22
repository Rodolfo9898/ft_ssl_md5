/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256_rounds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:35 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 18:29:45 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/*
** FIPS 180-4, 4.2.2: K, the constant of each round (k[0] is K0), see
** include/core/defines.h.
*/
static const t_sha256_tables	g_sha256_tables = {
{
	SHA256_K0, SHA256_K1, SHA256_K2, SHA256_K3,
	SHA256_K4, SHA256_K5, SHA256_K6, SHA256_K7,
	SHA256_K8, SHA256_K9, SHA256_K10, SHA256_K11,
	SHA256_K12, SHA256_K13, SHA256_K14, SHA256_K15,
	SHA256_K16, SHA256_K17, SHA256_K18, SHA256_K19,
	SHA256_K20, SHA256_K21, SHA256_K22, SHA256_K23,
	SHA256_K24, SHA256_K25, SHA256_K26, SHA256_K27,
	SHA256_K28, SHA256_K29, SHA256_K30, SHA256_K31,
	SHA256_K32, SHA256_K33, SHA256_K34, SHA256_K35,
	SHA256_K36, SHA256_K37, SHA256_K38, SHA256_K39,
	SHA256_K40, SHA256_K41, SHA256_K42, SHA256_K43,
	SHA256_K44, SHA256_K45, SHA256_K46, SHA256_K47,
	SHA256_K48, SHA256_K49, SHA256_K50, SHA256_K51,
	SHA256_K52, SHA256_K53, SHA256_K54, SHA256_K55,
	SHA256_K56, SHA256_K57, SHA256_K58, SHA256_K59,
	SHA256_K60, SHA256_K61, SHA256_K62, SHA256_K63
}
};

/* A 32-bit word, most significant byte first */
static uint32_t	read_u32_be(const unsigned char *src)
{
	uint32_t	value;
	int			index;

	value = 0;
	index = 0;
	while (index < WORD_SIZE)
	{
		value = value * BYTE_RANGE + src[index];
		index++;
	}
	return (value);
}

/*
** The message schedule (FIPS 180-4, 6.2.2): words 16 to 63 from the earlier
** ones. sigma0 and sigma1 are the parity of two rotations and a shift of
** W[t - 15] and W[t - 2].
*/
static void	build_schedule(uint32_t *w)
{
	int			index;
	uint32_t	w15;
	uint32_t	w2;
	uint32_t	sigma0;
	uint32_t	sigma1;

	index = SHA256_BLOCK_WORDS;
	while (index < SHA256_ROUNDS)
	{
		w15 = w[index - SCHEDULE_LAG_S0];
		w2 = w[index - SCHEDULE_LAG_S1];
		sigma0 = parity(rotate_right32(w15, SCHEDULE_S0_ROT1),
				rotate_right32(w15, SCHEDULE_S0_ROT2),
				shift_right32(w15, SCHEDULE_S0_SHIFT));
		sigma1 = parity(rotate_right32(w2, SCHEDULE_S1_ROT1),
				rotate_right32(w2, SCHEDULE_S1_ROT2),
				shift_right32(w2, SCHEDULE_S1_SHIFT));
		w[index] = w[index - SCHEDULE_LAG_FAR] + sigma0
			+ w[index - SCHEDULE_LAG_ADD] + sigma1;
		index++;
	}
}

/*
** One round on the working variables a to h (v), with the word of the round
** (FIPS 180-4, 6.2.2): big_sigma1 of e and big_sigma0 of a are the parity of
** three rotations, then choose (e picks f or g) and majority (of a, b, c).
*/
static void	round_step(uint32_t *v, uint32_t word, int round)
{
	uint32_t	big_sigma1;
	uint32_t	big_sigma0;
	uint32_t	temp1;
	uint32_t	temp2;

	big_sigma1 = parity(rotate_right32(v[SHA_E], ROUND_S1_ROT1),
			rotate_right32(v[SHA_E], ROUND_S1_ROT2),
			rotate_right32(v[SHA_E], ROUND_S1_ROT3));
	big_sigma0 = parity(rotate_right32(v[SHA_A], ROUND_S0_ROT1),
			rotate_right32(v[SHA_A], ROUND_S0_ROT2),
			rotate_right32(v[SHA_A], ROUND_S0_ROT3));
	temp1 = v[SHA_H] + big_sigma1 + choose(v[SHA_E], v[SHA_F], v[SHA_G])
		+ g_sha256_tables.k[round] + word;
	temp2 = big_sigma0 + majority(v[SHA_A], v[SHA_B], v[SHA_C]);
	v[SHA_H] = v[SHA_G];
	v[SHA_G] = v[SHA_F];
	v[SHA_F] = v[SHA_E];
	v[SHA_E] = v[SHA_D] + temp1;
	v[SHA_D] = v[SHA_C];
	v[SHA_C] = v[SHA_B];
	v[SHA_B] = v[SHA_A];
	v[SHA_A] = temp1 + temp2;
}

/* One block of 64 bytes: its 16 words, the message schedule, 64 rounds */
void	sha256_process_block(t_sha256_ctx *ctx, const unsigned char *block)
{
	uint32_t	words[SHA256_ROUNDS];
	uint32_t	state[SHA256_STATE_WORDS];
	int			index;

	index = 0;
	while (index < SHA256_BLOCK_WORDS)
	{
		words[index] = read_u32_be(block + index * WORD_SIZE);
		index++;
	}
	build_schedule(words);
	ft_memcpy(state, ctx->h, sizeof(state));
	index = 0;
	while (index < SHA256_ROUNDS)
	{
		round_step(state, words[index], index);
		index++;
	}
	index = 0;
	while (index < SHA256_STATE_WORDS)
	{
		ctx->h[index] += state[index];
		index++;
	}
}
