/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:13 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 16:10:55 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/*
** Every function here returns 0 as soon as a write fails (a full disk, a
** closed output), so that the command reports it and fails.
*/

/* The digest in lowercase hexadecimal, in one write */
static int	write_hex(const unsigned char *digest, size_t len)
{
	static const char	hex[] = HEX_DIGITS;
	char				out[DIGEST_MAX_SIZE * HEX_PER_BYTE];
	size_t				index;

	if (len > DIGEST_MAX_SIZE)
		len = DIGEST_MAX_SIZE;
	index = 0;
	while (index < len)
	{
		out[index * HEX_PER_BYTE] = hex[digest[index] >> NIBBLE_BITS];
		out[index * HEX_PER_BYTE + 1] = hex[digest[index] & NIBBLE_MASK];
		index++;
	}
	return (write_all(STDOUT_FILENO, out, len * HEX_PER_BYTE));
}

/* The label of a line: a string of -s between quotes, the rest as it is */
static int	write_label(const t_digest_print *print)
{
	int	quoted;

	quoted = (print->source_kind == DIGEST_SOURCE_STRING);
	if (quoted && !write_str(STDOUT_FILENO, "\""))
		return (0);
	if (print->label != NULL
		&& !write_all(STDOUT_FILENO, print->label, print->label_len))
		return (0);
	return (!quoted || write_str(STDOUT_FILENO, "\""));
}

/* -r: the digest, then what was hashed */
static int	print_reverse(const t_digest_algorithm *algorithm,
	const t_digest_print *print)
{
	return (write_hex(print->digest, algorithm->digest_size)
		&& write_str(STDOUT_FILENO, " ")
		&& write_label(print)
		&& write_str(STDOUT_FILENO, "\n"));
}

static int	print_named(const t_digest_algorithm *algorithm,
	const t_digest_print *print)
{
	return (write_str(STDOUT_FILENO, algorithm->display_name)
		&& write_str(STDOUT_FILENO, " (")
		&& write_label(print)
		&& write_str(STDOUT_FILENO, ") = "));
}

/*
** -q: the digest alone. -r applies to strings and files, never to the
** standard input, which is always "(stdin)= " or ("its text")= first.
*/
int	format_digest_output(const t_digest_algorithm *algorithm,
	const t_digest_options *options, const t_digest_print *print)
{
	int	ok;

	if (options->reverse && !options->quiet
		&& print->source_kind != DIGEST_SOURCE_STDIN_DEFAULT
		&& print->source_kind != DIGEST_SOURCE_STDIN_P)
		return (print_reverse(algorithm, print));
	if (options->quiet)
		ok = 1;
	else if (print->source_kind == DIGEST_SOURCE_STDIN_DEFAULT)
		ok = write_str(STDOUT_FILENO, "(stdin)= ");
	else if (print->source_kind == DIGEST_SOURCE_STDIN_P)
		ok = (write_str(STDOUT_FILENO, "(\"") && write_label(print)
				&& write_str(STDOUT_FILENO, "\")= "));
	else
		ok = print_named(algorithm, print);
	return (ok && write_hex(print->digest, algorithm->digest_size)
		&& write_str(STDOUT_FILENO, "\n"));
}
