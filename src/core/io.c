/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:10:03 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/15 23:10:04 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static int	grow_buffer(t_buffer *buffer, size_t len)
{
	unsigned char	*new_data;
	size_t			new_capacity;
	size_t			index;

	new_capacity = buffer->capacity;
	if (new_capacity == 0)
		new_capacity = 1024;
	while (new_capacity < buffer->len + len)
		new_capacity *= 2;
	new_data = (unsigned char *)malloc(new_capacity);
	if (new_data == NULL)
		return (errno = ENOMEM, 0);
	index = 0;
	while (index < buffer->len)
	{
		new_data[index] = buffer->data[index];
		index++;
	}
	free(buffer->data);
	buffer->data = new_data;
	buffer->capacity = new_capacity;
	return (1);
}

int	buffer_append(t_buffer *buffer, const unsigned char *data, size_t len)
{
	if (len == 0)
		return (1);
	if (buffer->len + len > buffer->capacity && !grow_buffer(buffer, len))
		return (0);
	ft_memcpy(buffer->data + buffer->len, data, len);
	buffer->len += len;
	return (1);
}

void	buffer_free(t_buffer *buffer)
{
	free(buffer->data);
	buffer->data = NULL;
	buffer->len = 0;
	buffer->capacity = 0;
}

int	read_stream(int fd, t_buffer *buffer)
{
	unsigned char	chunk[4096];
	ssize_t			bytes_read;

	bytes_read = read(fd, chunk, sizeof(chunk));
	while (bytes_read > 0)
	{
		if (!buffer_append(buffer, chunk, (size_t)bytes_read))
			return (0);
		bytes_read = read(fd, chunk, sizeof(chunk));
	}
	return (bytes_read >= 0);
}

int	read_file(const char *path, t_buffer *buffer)
{
	int	fd;
	int	ok;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (0);
	ok = read_stream(fd, buffer);
	close(fd);
	return (ok);
}
