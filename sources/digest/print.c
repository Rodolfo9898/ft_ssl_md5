/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:13 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 18:29:45 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/*
** Each put_ function writes only while ok is 1, and sets it to 0 when a write
** fails (a full disk, a closed output): the command then reports it and fails.
*/
static void	put_str(int *ok, const char *str)
{
	if (*ok)
		*ok = write_str(STDOUT_FILENO, str);
}

/* The digest in lowercase hexadecimal, in one write */
static void	put_hex(int *ok, const unsigned char *digest, size_t len)
{
	static const char	hex[] = HEX_DIGITS;
	char				out[DIGEST_MAX_SIZE * HEX_PER_BYTE];
	size_t				index;

	if (len > DIGEST_MAX_SIZE)
		len = DIGEST_MAX_SIZE;
	ft_bzero(out, sizeof(out));
	index = 0;
	while (index < len)
	{
		out[index * HEX_PER_BYTE] = hex[digest[index] / HEX_BASE];
		out[index * HEX_PER_BYTE + 1] = hex[digest[index] % HEX_BASE];
		index++;
	}
	if (*ok)
		*ok = write_all(STDOUT_FILENO, out, len * HEX_PER_BYTE);
}

/* What was hashed: a string of -s between quotes, the rest as it is */
static void	put_label(int *ok, const t_digest_print *print)
{
	if (print->source_kind == DIGEST_SOURCE_STRING)
		put_str(ok, "\"");
	if (*ok && print->label != NULL)
		*ok = write_all(STDOUT_FILENO, print->label, print->label_len);
	if (print->source_kind == DIGEST_SOURCE_STRING)
		put_str(ok, "\"");
}

/* What comes before the digest: (stdin)= , ("its text")=  or MD5 (...) =  */
static void	put_prefix(int *ok, const t_digest_algorithm *algorithm,
	const t_digest_print *print)
{
	if (print->source_kind == DIGEST_SOURCE_STDIN_DEFAULT)
		put_str(ok, "(stdin)= ");
	else if (print->source_kind == DIGEST_SOURCE_STDIN_P)
	{
		put_str(ok, "(\"");
		put_label(ok, print);
		put_str(ok, "\")= ");
	}
	else
	{
		put_str(ok, algorithm->display_name);
		put_str(ok, " (");
		put_label(ok, print);
		put_str(ok, ") = ");
	}
}

/*
** One line: -q prints the digest alone, -r the digest then what was hashed
** (for strings and files only: the standard input keeps its prefix), and
** otherwise the prefix then the digest. 0 when a write failed.
*/
int	format_digest_output(const t_digest_algorithm *algorithm,
	const t_digest_options *options, const t_digest_print *print)
{
	int	ok;
	int	reverse;

	ok = 1;
	reverse = (options->reverse && !options->quiet
			&& print->source_kind != DIGEST_SOURCE_STDIN_DEFAULT
			&& print->source_kind != DIGEST_SOURCE_STDIN_P);
	if (!options->quiet && !reverse)
		put_prefix(&ok, algorithm, print);
	put_hex(&ok, print->digest, algorithm->digest_size);
	if (reverse)
	{
		put_str(&ok, " ");
		put_label(&ok, print);
	}
	put_str(&ok, "\n");
	return (ok);
}
