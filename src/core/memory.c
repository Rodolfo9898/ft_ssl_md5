/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 20:14:32 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/15 20:14:33 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	ft_memcpy(void *dst, const void *src, size_t len)
{
	size_t				index;
	unsigned char		*out;
	const unsigned char	*in;

	out = (unsigned char *)dst;
	in = (const unsigned char *)src;
	index = 0;
	while (index < len)
	{
		out[index] = in[index];
		index++;
	}
}

void	ft_memset(void *dst, int c, size_t len)
{
	size_t			index;
	unsigned char	*out;

	out = (unsigned char *)dst;
	index = 0;
	while (index < len)
	{
		out[index] = (unsigned char)c;
		index++;
	}
}
