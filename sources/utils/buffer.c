/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 16:59:56 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 16:10:55 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

static int	grow_buffer(t_buffer *buffer, size_t len)
{
	unsigned char	*new_data;
	size_t			new_capacity;

	new_capacity = buffer->capacity;
	if (new_capacity == 0)
		new_capacity = BUFFER_FIRST_SIZE;
	while (new_capacity < buffer->len + len)
		new_capacity *= GROWTH_FACTOR;
	new_data = (unsigned char *)malloc(new_capacity);
	if (new_data == NULL)
		return (errno = ENOMEM, 0);
	if (buffer->len > 0)
		ft_memcpy(new_data, buffer->data, buffer->len);
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
	ft_bzero(buffer, sizeof(*buffer));
}

/* Everything fd gives until its end: 0 on a read error or out of memory */
int	read_stream(int fd, t_buffer *buffer)
{
	unsigned char	chunk[READ_SIZE];
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
