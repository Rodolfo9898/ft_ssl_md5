/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:09 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 18:05:35 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/* -s: the string itself is hashed, and shown between quotes */
static int	process_digest_string(t_digest_runtime *runtime, char *value)
{
	t_buffer		buffer;
	t_digest_print	print;

	runtime->source_given = 1;
	buffer.data = (unsigned char *)value;
	buffer.len = ft_strlen(value);
	buffer.capacity = buffer.len;
	print.source_kind = DIGEST_SOURCE_STRING;
	print.label = value;
	print.label_len = buffer.len;
	print.digest = NULL;
	return (digest_and_print(runtime, &print, &buffer));
}

static int	process_string_option(t_digest_runtime *runtime, int argc,
	char **argv, int *index)
{
	if (*index + 1 >= argc)
	{
		ft_putstr_fd("ft_ssl: ", STDERR_FILENO);
		ft_putstr_fd(runtime->algorithm->name, STDERR_FILENO);
		ft_putendl_fd(": option requires an argument -- s", STDERR_FILENO);
		runtime->had_error = 1;
		return (0);
	}
	*index += 1;
	return (process_digest_string(runtime, argv[*index]));
}

/* A flag, or the first file: from then on, every argument is a file */
static int	handle_digest_arg(t_digest_runtime *runtime, int argc,
	char **argv, int *index)
{
	char	*arg;

	arg = argv[*index];
	if (ft_strncmp(arg, FLAG_PRINT, sizeof(FLAG_PRINT)) == 0)
		return (process_digest_stdin(runtime, DIGEST_SOURCE_STDIN_P));
	if (ft_strncmp(arg, FLAG_STRING, sizeof(FLAG_STRING)) == 0)
		return (process_string_option(runtime, argc, argv, index));
	if (ft_strncmp(arg, FLAG_QUIET, sizeof(FLAG_QUIET)) == 0)
		runtime->options.quiet = 1;
	else if (ft_strncmp(arg, FLAG_REVERSE, sizeof(FLAG_REVERSE)) == 0)
		runtime->options.reverse = 1;
	else
	{
		runtime->parsed_operand = 1;
		return (process_digest_file(runtime, arg));
	}
	return (1);
}

/*
** Every digest command (md5, sha256, whirlpool) runs here with its algorithm.
** The arguments are read in order; the standard input is read only when no
** file, -s or -p was given, even when that file could not be read. status:
** 0 once an error (out of memory, a write that fails) makes going on
** pointless.
*/
int	run_digest_command(const t_digest_algorithm *algorithm, int argc,
	char **argv)
{
	t_digest_runtime	runtime;
	int					index;
	int					status;

	ft_bzero(&runtime, sizeof(runtime));
	runtime.algorithm = algorithm;
	index = 0;
	status = 1;
	while (index < argc && status)
	{
		if (!runtime.parsed_operand)
			status = handle_digest_arg(&runtime, argc, argv, &index);
		else
			status = process_digest_file(&runtime, argv[index]);
		index++;
	}
	if (status && !runtime.source_given)
		status = process_digest_stdin(&runtime, DIGEST_SOURCE_STDIN_DEFAULT);
	buffer_free(&runtime.stdin_buffer);
	if (!status || runtime.had_error)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
