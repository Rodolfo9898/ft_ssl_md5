/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whirlpool_round.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:42 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/15 23:27:43 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

uint64_t	whirlpool_transform(const uint64_t src[8], int index);
uint64_t	whirlpool_round_constant(int round);

static uint64_t	read_word(const unsigned char *buffer, int block_index)
{
	uint64_t	word;
	int			byte;

	word = 0;
	byte = -1;
	while (++byte < 8)
		word = (word << 8) | buffer[block_index * 8 + byte];
	return (word);
}

static void	apply_round(uint64_t k[8], uint64_t l[8], uint64_t state[8],
	int round)
{
	int	index;

	index = -1;
	while (++index < 8)
		l[index] = whirlpool_transform(k, index);
	l[0] ^= whirlpool_round_constant(round);
	index = -1;
	while (++index < 8)
		k[index] = l[index];
	index = -1;
	while (++index < 8)
		l[index] = whirlpool_transform(state, index) ^ k[index];
	index = -1;
	while (++index < 8)
		state[index] = l[index];
}

void	whirlpool_process_buffer(uint64_t hash[8], const unsigned char *buffer)
{
	uint64_t	block[8];
	uint64_t	k[8];
	uint64_t	l[8];
	uint64_t	state[8];
	int			index;

	index = -1;
	while (++index < 8)
	{
		block[index] = read_word(buffer, index);
		k[index] = hash[index];
		state[index] = block[index] ^ k[index];
	}
	index = 0;
	while (++index <= 10)
		apply_round(k, l, state, index);
	index = -1;
	while (++index < 8)
		hash[index] ^= state[index] ^ block[index];
}
