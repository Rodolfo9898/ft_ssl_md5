/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 20:14:12 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/15 20:14:13 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <stdlib.h>

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len] != '\0')
		len++;
	return (len);
}

int	ft_is_streq(const char *a, const char *b)
{
	size_t	index;

	index = 0;
	while (a[index] != '\0' && b[index] != '\0')
	{
		if (a[index] != b[index])
			return (0);
		index++;
	}
	return (a[index] == '\0' && b[index] == '\0');
}

int	ft_strcmp(const char *a, const char *b)
{
	size_t	index;

	index = 0;
	while (a[index] != '\0' && b[index] != '\0' && a[index] == b[index])
		index++;
	return ((unsigned char)a[index] - (unsigned char)b[index]);
}

char	*ft_strdup(const char *src)
{
	size_t	len;
	size_t	index;
	char	*dup;

	len = ft_strlen(src);
	dup = (char *)malloc(len + 1);
	if (dup == NULL)
		return (NULL);
	index = 0;
	while (index < len)
	{
		dup[index] = src[index];
		index++;
	}
	dup[index] = '\0';
	return (dup);
}
