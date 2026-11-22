/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whirlpool_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:39 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 16:10:55 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/* Only with -D BONUS_MODE=1 (make bonus): see include/core/defines.h */
#if BONUS_MODE

/* The message length in bits, on the WHIRLPOOL_LENGTH_SIZE bytes it takes */
static void	build_length(unsigned char *bit_length, size_t len)
{
	uint64_t	bits;
	int			index;

	ft_bzero(bit_length, WHIRLPOOL_LENGTH_SIZE);
	bits = (uint64_t)len * BYTE_BITS;
	index = 0;
	while (index < LENGTH_SIZE)
	{
		bit_length[WHIRLPOOL_LENGTH_SIZE - 1 - index] = bits & BYTE_MASK;
		bits >>= BYTE_BITS;
		index++;
	}
}

/*
** The last bytes of the message, PAD_BYTE, zeros, and the length: one more
** block when they leave no room for the length.
*/
static void	process_tail(uint64_t *hash, unsigned char *block,
	const unsigned char *data, size_t *tail)
{
	unsigned char	bit_length[WHIRLPOOL_LENGTH_SIZE];

	ft_bzero(block, BLOCK_SIZE);
	if (tail[TAIL_LEN] > 0)
		ft_memcpy(block, data + tail[TAIL_START], tail[TAIL_LEN]);
	block[tail[TAIL_LEN]] |= PAD_BYTE;
	if (tail[TAIL_LEN] + 1 > BLOCK_SIZE - WHIRLPOOL_LENGTH_SIZE)
	{
		whirlpool_process_buffer(hash, block);
		ft_bzero(block, BLOCK_SIZE);
	}
	build_length(bit_length, tail[TAIL_START] + tail[TAIL_LEN]);
	ft_memcpy(block + BLOCK_SIZE - WHIRLPOOL_LENGTH_SIZE, bit_length,
		WHIRLPOOL_LENGTH_SIZE);
	whirlpool_process_buffer(hash, block);
}

/* Each 64-bit word of the hash, most significant byte first */
static void	write_digest(unsigned char *digest, const uint64_t *hash)
{
	int			word;
	int			byte;
	uint64_t	value;

	word = -1;
	while (++word < WHIRLPOOL_WORDS)
	{
		value = hash[word];
		byte = WHIRLPOOL_WORD_SIZE;
		while (byte-- > 0)
		{
			digest[word * WHIRLPOOL_WORD_SIZE + byte] = value & BYTE_MASK;
			value >>= BYTE_BITS;
		}
	}
}

/* Whirlpool (ISO/IEC 10118-3): the whole blocks, then the padded tail */
static int	whirlpool_compute(const unsigned char *data, size_t len,
	unsigned char *digest)
{
	uint64_t		hash[WHIRLPOOL_WORDS];
	unsigned char	block[BLOCK_SIZE];
	size_t			tail[TAIL_FIELDS];

	ft_bzero(hash, sizeof(hash));
	tail[TAIL_START] = 0;
	while (tail[TAIL_START] + BLOCK_SIZE <= len)
	{
		whirlpool_process_buffer(hash, data + tail[TAIL_START]);
		tail[TAIL_START] += BLOCK_SIZE;
	}
	tail[TAIL_LEN] = len - tail[TAIL_START];
	process_tail(hash, block, data, tail);
	write_digest(digest, hash);
	return (1);
}

/* The whirlpool command: the digest pipeline of digest/, with Whirlpool */
int	run_whirlpool(int argc, char **argv)
{
	static const t_digest_algorithm	whirlpool = {"whirlpool", "WHIRLPOOL",
		WHIRLPOOL_DIGEST_SIZE, whirlpool_compute};

	return (run_digest_command(&whirlpool, argc, argv));
}

#endif
