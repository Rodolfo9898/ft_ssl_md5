/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   digest_options.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:59:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/15 23:59:01 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static int	process_string_option(t_digest_runtime *runtime, int argc,
	char **argv, int *index)
{
	if (*index + 1 >= argc)
	{
		ft_putstr_fd(2, "ft_ssl: ");
		ft_putstr_fd(2, runtime->algorithm->name);
		ft_putendl_fd(2, ": option requires an argument -- s");
		runtime->had_error = 1;
		return (0);
	}
	*index += 1;
	return (process_digest_string(runtime, argv[*index]));
}

int	handle_digest_arg(t_digest_runtime *runtime, int argc,
	char **argv, int *index)
{
	if (ft_is_streq(argv[*index], "-p"))
		return (process_digest_stdin(runtime, 1));
	if (ft_is_streq(argv[*index], "-q"))
		return (runtime->options.quiet = 1, 1);
	if (ft_is_streq(argv[*index], "-r"))
		return (runtime->options.reverse = 1, 1);
	if (ft_is_streq(argv[*index], "-s"))
		return (process_string_option(runtime, argc, argv, index));
	runtime->parsed_operand = 1;
	process_digest_file(runtime, argv[*index]);
	return (1);
}
