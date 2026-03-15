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
