/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   digest_output.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:13 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/15 23:27:14 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <unistd.h>

static void	write_label(const char *label, size_t label_len)
{
	if (label != NULL && label_len > 0)
		write_all(STDOUT_FILENO, label, label_len);
}

static void	print_reverse(const t_digest_algorithm *algorithm,
	const t_digest_print *print)
{
	ft_puthex_fd(STDOUT_FILENO, print->digest, algorithm->digest_size);
	write_all(STDOUT_FILENO, " ", 1);
	if (print->source_kind == DIGEST_SOURCE_STRING)
		write_all(STDOUT_FILENO, "\"", 1);
	write_label(print->label, print->label_len);
	if (print->source_kind == DIGEST_SOURCE_STRING)
		write_all(STDOUT_FILENO, "\"", 1);
	write_all(STDOUT_FILENO, "\n", 1);
}

static void	print_named(const t_digest_algorithm *algorithm,
	const t_digest_print *print)
{
	ft_putstr_fd(STDOUT_FILENO, algorithm->display_name);
	write_all(STDOUT_FILENO, " (", 2);
	if (print->source_kind == DIGEST_SOURCE_STRING)
		write_all(STDOUT_FILENO, "\"", 1);
	write_label(print->label, print->label_len);
	if (print->source_kind == DIGEST_SOURCE_STRING)
		write_all(STDOUT_FILENO, "\"", 1);
	ft_putstr_fd(STDOUT_FILENO, ") = ");
}

static void	print_quiet(const t_digest_algorithm *algorithm,
	const t_digest_print *print)
{
	ft_puthex_fd(STDOUT_FILENO, print->digest, algorithm->digest_size);
	write_all(STDOUT_FILENO, "\n", 1);
}

void	format_digest_output(const t_digest_algorithm *algorithm,
	const t_digest_options *options, const t_digest_print *print)
{
	if (options->quiet)
		return (print_quiet(algorithm, print));
	if (options->reverse && print->source_kind != DIGEST_SOURCE_STDIN_DEFAULT
		&& print->source_kind != DIGEST_SOURCE_STDIN_P)
		return (print_reverse(algorithm, print));
	if (print->source_kind == DIGEST_SOURCE_STDIN_DEFAULT)
		ft_putstr_fd(STDOUT_FILENO, "(stdin)= ");
	else if (print->source_kind == DIGEST_SOURCE_STDIN_P)
	{
		write_all(STDOUT_FILENO, "(\"", 2);
		write_label(print->label, print->label_len);
		ft_putstr_fd(STDOUT_FILENO, "\")= ");
	}
	else
		print_named(algorithm, print);
	ft_puthex_fd(STDOUT_FILENO, print->digest, algorithm->digest_size);
	write_all(STDOUT_FILENO, "\n", 1);
}
