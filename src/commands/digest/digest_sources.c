/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   digest_sources.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:18 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/15 23:27:19 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <errno.h>
#include <unistd.h>

static size_t	trimmed_label_len(const t_buffer *buffer)
{
	size_t	len;

	len = buffer->len;
	if (len > 0 && buffer->data[len - 1] == '\n')
		len--;
	return (len);
}

static int	load_stdin_once(t_digest_runtime *runtime)
{
	if (!runtime->stdin_loaded)
	{
		if (!read_stream(STDIN_FILENO, &runtime->stdin_buffer))
			return (0);
		runtime->stdin_loaded = 1;
	}
	return (1);
}

static int	compute_and_print(t_digest_runtime *runtime,
	const t_digest_print *print, const t_buffer *buffer)
{
	unsigned char	digest[64];
	t_digest_print	output;

	if (!runtime->algorithm->compute(buffer->data, buffer->len, digest))
	{
		if (errno == 0)
			errno = ENOMEM;
		print_system_error(runtime->algorithm->name, "digest");
		return (0);
	}
	output = *print;
	output.digest = digest;
	format_digest_output(runtime->algorithm, &runtime->options, &output);
	return (1);
}

void	process_digest_file(t_digest_runtime *runtime, const char *path)
{
	t_buffer		buffer;
	t_digest_print	print;

	buffer.data = NULL;
	buffer.len = 0;
	buffer.capacity = 0;
	if (!read_file(path, &buffer))
		return (print_system_error(runtime->algorithm->name, path),
			runtime->had_error = 1, (void)0);
	print.source_kind = DIGEST_SOURCE_FILE;
	print.label = path;
	print.label_len = ft_strlen(path);
	print.digest = NULL;
	if (!compute_and_print(runtime, &print, &buffer))
		runtime->had_error = 1;
	else
		runtime->processed_any = 1;
	buffer_free(&buffer);
}

int	process_digest_stdin(t_digest_runtime *runtime, int mode)
{
	t_digest_print	print;

	if (!load_stdin_once(runtime))
		return (0);
	print.label = NULL;
	print.label_len = 0;
	if (mode == 0)
		print.source_kind = DIGEST_SOURCE_STDIN_DEFAULT;
	else
	{
		print.source_kind = DIGEST_SOURCE_STDIN_P;
		print.label = (const char *)runtime->stdin_buffer.data;
		print.label_len = trimmed_label_len(&runtime->stdin_buffer);
		if (runtime->options.quiet)
			write_all(STDOUT_FILENO, runtime->stdin_buffer.data,
				runtime->stdin_buffer.len);
	}
	print.digest = NULL;
	if (!compute_and_print(runtime, &print, &runtime->stdin_buffer))
		return (runtime->had_error = 1, 0);
	runtime->processed_any = 1;
	return (1);
}
