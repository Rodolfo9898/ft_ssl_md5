/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 18:22:29 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 18:26:25 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/*
** Moving the bits of a number n places is multiplying it by 2^n (to the left)
** or dividing it by 2^n (to the right). These helpers do it with arithmetic,
** on unsigned numbers, where a product too big for the type keeps only its
** low bits (it wraps modulo 2^32 or 2^64): exactly what the hashes need.
*/

/* 2^n, for n from 0 to 63: each power is twice the one before */
uint64_t	power_of_two(int n)
{
	static uint64_t	powers[WIDE_WORD_BITS];
	static int		ready;
	int				index;

	if (!ready)
	{
		powers[0] = 1;
		index = 1;
		while (index < WIDE_WORD_BITS)
		{
			powers[index] = powers[index - 1] * 2;
			index++;
		}
		ready = 1;
	}
	return (powers[n]);
}

/*
** The 32 bits of value turned n places to the left: the bits that leave on
** the left come back on the right. value * 2^n, on 64 bits, holds both parts:
** its low 32 bits (% 2^32) are the bits moved up, its high 32 bits (/ 2^32)
** the bits that left, and adding them puts each back in its place.
*/
uint32_t	rotate_left32(uint32_t value, int n)
{
	uint64_t	product;

	product = value * power_of_two(n);
	return (product % WORD_RANGE + product / WORD_RANGE);
}

/* Turning right by n is turning left by the rest of the word */
uint32_t	rotate_right32(uint32_t value, int n)
{
	return (rotate_left32(value, WORD_BITS - n));
}

/*
** The 32 bits of value moved n places to the right, the low bits lost:
** value / 2^n, computed as value * 2^(32 - n) / 2^32.
*/
uint32_t	shift_right32(uint32_t value, int n)
{
	uint64_t	product;

	product = value * power_of_two(WORD_BITS - n);
	return (product / WORD_RANGE);
}

/* The 64 bits of value turned n places to the right, n from 1 to 63 */
uint64_t	rotate_right64(uint64_t value, int n)
{
	uint64_t	moved_down;
	uint64_t	wrapped;

	moved_down = value / power_of_two(n);
	wrapped = value * power_of_two(WIDE_WORD_BITS - n);
	return (moved_down + wrapped);
}
