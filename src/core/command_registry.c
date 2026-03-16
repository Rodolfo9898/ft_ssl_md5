/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_registry.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:07:38 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/16 21:42:44 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

const t_command	*get_command_table(size_t *count)
{
	static const t_command	commands[] = {
	{
		"md5",
		COMMAND_GROUP_MESSAGE_DIGEST,
		run_digest_command,
		(const void *)get_md5_algorithm
	},
	{
		"sha256",
		COMMAND_GROUP_MESSAGE_DIGEST,
		run_digest_command,
		(const void *)get_sha256_algorithm
	},
	{
		"whirlpool",
		COMMAND_GROUP_MESSAGE_DIGEST,
		run_digest_command,
		(const void *)get_whirlpool_algorithm
	}
	};

	if (count != NULL)
		*count = sizeof(commands) / sizeof(commands[0]);
	return (commands);
}

const t_command	*find_command(const char *name)
{
	size_t			count;
	size_t			index;
	const t_command	*commands;

	commands = get_command_table(&count);
	index = 0;
	while (index < count)
	{
		if (ft_is_streq(commands[index].name, name))
			return (&commands[index]);
		index++;
	}
	return (NULL);
}

int	execute_command_argv(int argc, char **argv)
{
	const t_command	*command;

	if (argc <= 0)
		return (0);
	command = find_command(argv[0]);
	if (command == NULL)
	{
		print_invalid_command(argv[0]);
		return (1);
	}
	return (command->handler(command, argc - 1, argv + 1));
}
