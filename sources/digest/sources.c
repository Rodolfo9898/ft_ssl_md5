/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:18 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 18:05:35 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

static size_t	trimmed_label_len(const t_buffer *buffer)
{
	size_t	len;

	len = buffer->len;
	if (len > 0 && buffer->data[len - 1] == '\n')
		len--;
	return (len);
}

/* The standard input is read once, the first time -p or the default needs it */
static int	load_stdin_once(t_digest_runtime *runtime)
{
	if (!runtime->stdin_loaded)
	{
		if (!read_stream(STDIN_FILENO, &runtime->stdin_buffer))
		{
			print_system_error(runtime->algorithm->name, "stdin");
			runtime->had_error = 1;
			return (0);
		}
		runtime->stdin_loaded = 1;
	}
	return (1);
}

/*
** Hashes buffer and prints the line of print. 0 when the digest could not be
** computed (out of memory) or the line could not be written: the error is
** printed and recorded, and the command stops there.
*/
int	digest_and_print(t_digest_runtime *runtime, const t_digest_print *print,
	const t_buffer *buffer)
{
	unsigned char	digest[DIGEST_MAX_SIZE];
	t_digest_print	output;
	char			*failed;

	errno = 0;
	failed = NULL;
	output = *print;
	output.digest = digest;
	if (!runtime->algorithm->compute(buffer->data, buffer->len, digest))
		failed = "digest";
	else if (!format_digest_output(runtime->algorithm, &runtime->options,
			&output))
		failed = "stdout";
	if (failed == NULL)
		return (1);
	if (errno == 0)
		errno = ENOMEM;
	print_system_error(runtime->algorithm->name, failed);
	runtime->had_error = 1;
	return (0);
}

/*
** A file that cannot be read is an error of its own: the next arguments are
** still handled (return 1). It still counts as a source: no stdin after it.
*/
int	process_digest_file(t_digest_runtime *runtime, char *path)
{
	t_buffer		buffer;
	t_digest_print	print;
	int				ok;

	runtime->source_given = 1;
	ft_bzero(&buffer, sizeof(buffer));
	if (!read_file(path, &buffer))
	{
		print_system_error(runtime->algorithm->name, path);
		buffer_free(&buffer);
		runtime->had_error = 1;
		return (1);
	}
	ft_bzero(&print, sizeof(print));
	print.source_kind = DIGEST_SOURCE_FILE;
	print.label = path;
	print.label_len = ft_strlen(path);
	ok = digest_and_print(runtime, &print, &buffer);
	buffer_free(&buffer);
	return (ok);
}

/* The standard input: DIGEST_SOURCE_STDIN_DEFAULT, or -p (STDIN_P) */
int	process_digest_stdin(t_digest_runtime *runtime,
	t_digest_source_kind kind)
{
	t_digest_print	print;

	runtime->source_given = 1;
	if (!load_stdin_once(runtime))
		return (0);
	ft_bzero(&print, sizeof(print));
	print.source_kind = kind;
	if (kind == DIGEST_SOURCE_STDIN_P)
	{
		print.label = (const char *)runtime->stdin_buffer.data;
		print.label_len = trimmed_label_len(&runtime->stdin_buffer);
	}
	if (kind == DIGEST_SOURCE_STDIN_P && runtime->options.quiet)
	{
		if (!write_all(STDOUT_FILENO, runtime->stdin_buffer.data,
				runtime->stdin_buffer.len))
		{
			print_system_error(runtime->algorithm->name, "stdout");
			runtime->had_error = 1;
			return (0);
		}
	}
	return (digest_and_print(runtime, &print, &runtime->stdin_buffer));
}
