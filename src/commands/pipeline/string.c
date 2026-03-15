/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   digest_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 00:10:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/16 00:10:01 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <errno.h>

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

int	process_digest_string(t_digest_runtime *runtime, const char *value)
{
	t_buffer		buffer;
	t_digest_print	print;

	buffer.data = (unsigned char *)value;
	buffer.len = ft_strlen(value);
	buffer.capacity = buffer.len;
	print.source_kind = DIGEST_SOURCE_STRING;
	print.label = value;
	print.label_len = buffer.len;
	print.digest = NULL;
	if (!compute_and_print(runtime, &print, &buffer))
		return (runtime->had_error = 1, 0);
	runtime->processed_any = 1;
	return (1);
}
