#include "ft_ssl.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int	buffer_append(t_buffer *buffer, const unsigned char *data, size_t len)
{
	unsigned char	*new_data;
	size_t			new_capacity;
	size_t			index;

	if (len == 0)
		return (1);
	if (buffer->len + len <= buffer->capacity)
	{
		ft_memcpy(buffer->data + buffer->len, data, len);
		buffer->len += len;
		return (1);
	}
	new_capacity = buffer->capacity;
	if (new_capacity == 0)
		new_capacity = 1024;
	while (new_capacity < buffer->len + len)
		new_capacity *= 2;
	new_data = (unsigned char *)malloc(new_capacity);
	if (new_data == NULL)
	{
		errno = ENOMEM;
		return (0);
	}
	index = 0;
	while (index < buffer->len)
	{
		new_data[index] = buffer->data[index];
		index++;
	}
	ft_memcpy(new_data + buffer->len, data, len);
	free(buffer->data);
	buffer->data = new_data;
	buffer->capacity = new_capacity;
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

int	write_all(int fd, const void *data, size_t len)
{
	const unsigned char	*bytes;
	ssize_t				written;

	bytes = (const unsigned char *)data;
	while (len > 0)
	{
		written = write(fd, bytes, len);
		if (written < 0)
			return (0);
		bytes += written;
		len -= (size_t)written;
	}
	return (1);
}
