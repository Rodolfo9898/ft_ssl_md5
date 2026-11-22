/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whirlpool_block_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:42 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 18:29:45 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/* Only with -D BONUS_MODE=1 (make bonus): see include/core/defines.h */
#if BONUS_MODE

/* A 64-bit word of the block, most significant byte first */
static uint64_t	read_word(const unsigned char *buffer, int word)
{
	uint64_t	value;
	int			byte;

	buffer += word * WHIRLPOOL_WORD_SIZE;
	value = 0;
	byte = 0;
	while (byte < WHIRLPOOL_WORD_SIZE)
	{
		value = value * BYTE_RANGE + buffer[byte];
		byte++;
	}
	return (value);
}

/*
** One block of 64 bytes: WHIRLPOOL_ROUNDS rounds of the W cipher keyed by the
** hash, then Miyaguchi-Preneel, hash ^= W(block) ^ block.
*/
void	whirlpool_process_buffer(uint64_t *hash, const unsigned char *buffer)
{
	uint64_t	block[WHIRLPOOL_WORDS];
	uint64_t	key[WHIRLPOOL_WORDS];
	uint64_t	state[WHIRLPOOL_WORDS];
	int			index;

	index = 0;
	while (index < WHIRLPOOL_WORDS)
	{
		block[index] = read_word(buffer, index);
		key[index] = hash[index];
		state[index] = xor64(block[index], key[index]);
		index++;
	}
	index = 1;
	while (index <= WHIRLPOOL_ROUNDS)
	{
		whirlpool_round(key, state, index);
		index++;
	}
	index = 0;
	while (index < WHIRLPOOL_WORDS)
	{
		hash[index] = xor64(hash[index], xor64(state[index], block[index]));
		index++;
	}
}

#endif
