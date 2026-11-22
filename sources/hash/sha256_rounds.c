/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256_rounds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:35 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 16:57:15 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/*
** FIPS 180-4, 4.2.2: K, the constant of each round (the first 32 bits of the
** fractional parts of the cube roots of the first 64 primes). The ones that
** start with 0xB have a leading 0, as SHA256_H1 in defines.h.
*/
static const t_sha256_tables	g_sha256_tables = {
{
	0x428A2F98, 0x71374491, 0x0B5C0FBCF, 0xE9B5DBA5,
	0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
	0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,
	0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
	0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC,
	0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
	0x983E5152, 0xA831C66D, 0x0B00327C8, 0x0BF597FC7,
	0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
	0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
	0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
	0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3,
	0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
	0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5,
	0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
	0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
	0x90BEFFFA, 0xA4506CEB, 0x0BEF9A3F7, 0xC67178F2
}
};

static uint32_t	right_rotate(uint32_t value, uint32_t shift)
{
	return ((value >> shift) | (value << (WORD_BITS - shift)));
}

/* A 32-bit word, most significant byte first */
static uint32_t	read_u32_be(const unsigned char *src)
{
	uint32_t	value;
	int			index;

	value = 0;
	index = -1;
	while (++index < WORD_SIZE)
		value = (value << BYTE_BITS) | src[index];
	return (value);
}

/* The message schedule: the 16 words of the block, then 48 more from them */
static void	build_schedule(uint32_t *w, const unsigned char *block)
{
	int			index;
	uint32_t	s0;
	uint32_t	s1;

	index = -1;
	while (++index < SHA256_BLOCK_WORDS)
		w[index] = read_u32_be(block + index * WORD_SIZE);
	while (index < SHA256_ROUNDS)
	{
		s0 = right_rotate(w[index - SCHEDULE_LAG_S0], SCHEDULE_S0_ROT1)
			^ right_rotate(w[index - SCHEDULE_LAG_S0], SCHEDULE_S0_ROT2)
			^ (w[index - SCHEDULE_LAG_S0] >> SCHEDULE_S0_SHIFT);
		s1 = right_rotate(w[index - SCHEDULE_LAG_S1], SCHEDULE_S1_ROT1)
			^ right_rotate(w[index - SCHEDULE_LAG_S1], SCHEDULE_S1_ROT2)
			^ (w[index - SCHEDULE_LAG_S1] >> SCHEDULE_S1_SHIFT);
		w[index] = w[index - SCHEDULE_LAG_FAR] + s0
			+ w[index - SCHEDULE_LAG_ADD] + s1;
		index++;
	}
}

/* One round on the working variables a to h (v), with the word of the round */
static void	round_step(uint32_t *v, uint32_t word, int round)
{
	uint32_t	temp1;
	uint32_t	temp2;
	uint32_t	majority;

	temp1 = v[SHA_H] + (right_rotate(v[SHA_E], ROUND_S1_ROT1)
			^ right_rotate(v[SHA_E], ROUND_S1_ROT2)
			^ right_rotate(v[SHA_E], ROUND_S1_ROT3))
		+ ((v[SHA_E] & v[SHA_F]) ^ (~v[SHA_E] & v[SHA_G]))
		+ g_sha256_tables.k[round] + word;
	majority = (v[SHA_A] & v[SHA_B]) ^ (v[SHA_A] & v[SHA_C])
		^ (v[SHA_B] & v[SHA_C]);
	temp2 = (right_rotate(v[SHA_A], ROUND_S0_ROT1)
			^ right_rotate(v[SHA_A], ROUND_S0_ROT2)
			^ right_rotate(v[SHA_A], ROUND_S0_ROT3)) + majority;
	v[SHA_H] = v[SHA_G];
	v[SHA_G] = v[SHA_F];
	v[SHA_F] = v[SHA_E];
	v[SHA_E] = v[SHA_D] + temp1;
	v[SHA_D] = v[SHA_C];
	v[SHA_C] = v[SHA_B];
	v[SHA_B] = v[SHA_A];
	v[SHA_A] = temp1 + temp2;
}

/* One block of 64 bytes: its message schedule, then 64 rounds */
void	sha256_process_block(t_sha256_ctx *ctx, const unsigned char *block)
{
	uint32_t	words[SHA256_ROUNDS];
	uint32_t	state[SHA256_STATE_WORDS];
	int			index;

	build_schedule(words, block);
	ft_memcpy(state, ctx->h, sizeof(state));
	index = -1;
	while (++index < SHA256_ROUNDS)
		round_step(state, words[index], index);
	index = -1;
	while (++index < SHA256_STATE_WORDS)
		ctx->h[index] += state[index];
}
