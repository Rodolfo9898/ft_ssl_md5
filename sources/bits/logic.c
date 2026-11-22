/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 18:22:29 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 18:22:29 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/*
** The only bitwise operators of the program, one function per idea of the
** specifications. Each works on all the bits at once, and bit i of the result
** only depends on bit i of each argument: & is AND, | is OR, ^ is XOR (one or
** the other, not both), ~ is NOT (each bit flipped).
*/

/* Where x has a 1, take the bit of y; where x has a 0, the bit of z */
uint32_t	choose(uint32_t x, uint32_t y, uint32_t z)
{
	return ((x & y) | (~x & z));
}

/* Each bit is the one that at least two of x, y and z have */
uint32_t	majority(uint32_t x, uint32_t y, uint32_t z)
{
	return ((x & y) | (x & z) | (y & z));
}

/* Each bit is 1 when an odd number of x, y and z have a 1 there */
uint32_t	parity(uint32_t x, uint32_t y, uint32_t z)
{
	return (x ^ y ^ z);
}

/* The function of MD5's fourth round (RFC 1321, 3.4): y XOR (x OR NOT z) */
uint32_t	md5_i(uint32_t x, uint32_t y, uint32_t z)
{
	return (y ^ (x | ~z));
}

/* Each bit is 1 when exactly one of a and b has a 1 there (Whirlpool) */
uint64_t	xor64(uint64_t a, uint64_t b)
{
	return (a ^ b);
}
