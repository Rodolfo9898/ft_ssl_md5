/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 15:54:21 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 18:29:45 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/* The length in bits on LENGTH_SIZE bytes, in the order the hash uses */
static void	write_length(unsigned char *dst, uint64_t bit_len,
	t_byte_order order)
{
	int	index;

	index = 0;
	while (index < LENGTH_SIZE)
	{
		if (order == ORDER_BIG_ENDIAN)
			dst[LENGTH_SIZE - 1 - index] = bit_len % BYTE_RANGE;
		else
			dst[index] = bit_len % BYTE_RANGE;
		bit_len /= BYTE_RANGE;
		index++;
	}
}

/*
** MD5 and SHA-256 pad the same way: the message, PAD_BYTE, zeros up to the
** last LENGTH_SIZE bytes of a block, then its length. The padded copy is
** allocated: NULL when malloc fails, with errno at ENOMEM.
*/
unsigned char	*pad_message(const unsigned char *data, size_t len,
	size_t *padded_len, t_byte_order order)
{
	unsigned char	*buffer;
	size_t			new_len;

	new_len = len + 1;
	while ((new_len % BLOCK_SIZE) != BLOCK_SIZE - LENGTH_SIZE)
		new_len++;
	*padded_len = new_len + LENGTH_SIZE;
	buffer = (unsigned char *)malloc(*padded_len);
	if (buffer == NULL)
	{
		errno = ENOMEM;
		return (NULL);
	}
	if (len > 0)
		ft_memcpy(buffer, data, len);
	ft_bzero(buffer + len, new_len - len);
	buffer[len] = PAD_BYTE;
	write_length(buffer + new_len, (uint64_t)len * BYTE_BITS, order);
	return (buffer);
}
