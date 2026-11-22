/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whirlpool_build_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 18:26:25 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 18:26:25 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/* Only with -D BONUS_MODE=1 (make bonus): see include/core/defines.h */
#if BONUS_MODE

/*
** x times coefficient in GF(2^8): for each bit of coefficient, from the
** lowest (coefficient % 2), x is added (XOR) when the bit is 1, then doubled.
** A doubled x that reaches GF_OVERFLOW (9 bits) is reduced by the polynomial.
*/
static uint32_t	gf_multiply(uint32_t x, uint32_t coefficient)
{
	uint32_t	product;

	product = 0;
	while (coefficient != 0)
	{
		if (coefficient % 2 == 1)
			product = xor64(product, x);
		x = x * 2;
		if (x >= GF_OVERFLOW)
			x = xor64(x, GF_POLY);
		coefficient = coefficient / 2;
	}
	return (product);
}

/* Byte number index of value, counting from the least significant one */
static unsigned char	byte_of(uint64_t value, int index)
{
	return ((value / power_of_two(index * BYTE_BITS)) % BYTE_RANGE);
}

/*
** The eight tables: in the first one, each S-box byte times the circulant
** row, one byte after the other; in each next one, the entry of the previous
** table turned right by a byte.
*/
static void	build_tables(t_whirlpool_tables *t)
{
	int	x;
	int	col;
	int	table;

	x = 0;
	while (x < SBOX_SIZE)
	{
		col = 0;
		while (col < WHIRLPOOL_WORD_SIZE)
		{
			t->c[0][x] = t->c[0][x] * BYTE_RANGE
				+ gf_multiply(t->sbox[x], t->circulant[col]);
			col++;
		}
		table = 1;
		while (table < WHIRLPOOL_TABLES)
		{
			t->c[table][x] = rotate_right64(t->c[table - 1][x], BYTE_BITS);
			table++;
		}
		x++;
	}
}

/*
** The constant of each round: byte col (from the top) of entry col of table
** col, eight entries further each round. The bytes do not overlap, so adding
** them puts each in its place.
*/
static void	init_round_constants(t_whirlpool_tables *t)
{
	int	round;
	int	col;
	int	place;

	round = 1;
	while (round <= WHIRLPOOL_ROUNDS)
	{
		t->rc[round] = 0;
		col = 0;
		while (col < WHIRLPOOL_TABLES)
		{
			place = WHIRLPOOL_TABLES - 1 - col;
			t->rc[round] += byte_of(t->c[col][WHIRLPOOL_WORD_SIZE
					* (round - 1) + col], place)
				* power_of_two(place * BYTE_BITS);
			col++;
		}
		round++;
	}
}

/* The tables and the round constants, from the S-box and the circulant row */
void	whirlpool_build(t_whirlpool_tables *t)
{
	build_tables(t);
	init_round_constants(t);
	t->initialized = 1;
}

#endif
