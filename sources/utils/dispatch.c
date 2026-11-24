/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 16:59:56 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 16:57:15 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/*
** The command table: one row per command, its name, its family and the
** function that runs it. The lookup and the list of commands read it, so a
** new command is a new row. make bonus (-D BONUS_MODE=1) adds whirlpool.
*/
#if BONUS_MODE

static const t_command	g_commands[] = {
{MD5_NAME, COMMAND_GROUP_MESSAGE_DIGEST, run_md5},
{SHA256_NAME, COMMAND_GROUP_MESSAGE_DIGEST, run_sha256},
{WHIRLPOOL_NAME, COMMAND_GROUP_MESSAGE_DIGEST, run_whirlpool}
};

#else

static const t_command	g_commands[] = {
{MD5_NAME, COMMAND_GROUP_MESSAGE_DIGEST, run_md5},
{SHA256_NAME, COMMAND_GROUP_MESSAGE_DIGEST, run_sha256}
};

#endif

static void	print_command_list(void)
{
	size_t	index;

	ft_putendl_fd("Commands:", STDERR_FILENO);
	index = 0;
	while (index < sizeof(g_commands) / sizeof(g_commands[0]))
	{
		ft_putendl_fd(g_commands[index].name, STDERR_FILENO);
		index++;
	}
	ft_putendl_fd("Flags:", STDERR_FILENO);
	ft_putendl_fd(MSG_FLAGS, STDERR_FILENO);
}

static const t_command	*find_command(const char *name)
{
	size_t	index;

	index = 0;
	while (index < sizeof(g_commands) / sizeof(g_commands[0]))
	{
		if (ft_strncmp(g_commands[index].name, name,
				ft_strlen(g_commands[index].name) + 1) == 0)
			return (&g_commands[index]);
		index++;
	}
	return (NULL);
}

/* argv[0] is the command: the arguments after it go to its function */
int	execute_command_argv(int argc, char **argv)
{
	const t_command	*command;

	if (argc <= 0)
		return (EXIT_SUCCESS);
	command = find_command(argv[0]);
	if (command == NULL)
	{
		ft_putstr_fd("ft_ssl: Error: '", STDERR_FILENO);
		ft_putstr_fd(argv[0], STDERR_FILENO);
		ft_putendl_fd("' is an invalid command.", STDERR_FILENO);
		print_command_list();
		return (EXIT_FAILURE);
	}
	return (command->handler(argc - 1, argv + 1));
}
