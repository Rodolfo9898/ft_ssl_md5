/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   digest_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:09 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/15 23:27:10 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static void	init_runtime(t_digest_runtime *runtime,
	const t_digest_algorithm *algorithm)
{
	runtime->algorithm = algorithm;
	runtime->options.quiet = 0;
	runtime->options.reverse = 0;
	runtime->parsed_operand = 0;
	runtime->processed_any = 0;
	runtime->had_error = 0;
	runtime->stdin_loaded = 0;
	runtime->stdin_buffer.data = NULL;
	runtime->stdin_buffer.len = 0;
	runtime->stdin_buffer.capacity = 0;
}

int	run_digest_command(const t_command *command, int argc, char **argv)
{
	const t_digest_algorithm	*(*get_algorithm)(void);
	t_digest_runtime			runtime;
	int							index;
	int							status;

	get_algorithm = (const t_digest_algorithm *(*)(void))command->data;
	init_runtime(&runtime, get_algorithm());
	index = 0;
	status = 1;
	while (index < argc && status)
	{
		if (!runtime.parsed_operand)
			status = handle_digest_arg(&runtime, argc, argv, &index);
		else
			process_digest_file(&runtime, argv[index]);
		index++;
	}
	if (status && !runtime.processed_any)
		status = process_digest_stdin(&runtime, 0);
	buffer_free(&runtime.stdin_buffer);
	if (!status || runtime.had_error)
		return (1);
	return (0);
}
