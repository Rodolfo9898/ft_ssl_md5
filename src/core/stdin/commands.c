/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdin_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:06:17 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/15 23:06:18 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <stdlib.h>
#include <unistd.h>

void	free_argv_tokens(char **argv, int argc)
{
	int	index;

	index = 0;
	while (index < argc)
	{
		free(argv[index]);
		index++;
	}
	free(argv);
}

static int	read_command_line(t_buffer *buffer)
{
	char	ch;
	int		has_data;
	ssize_t	bytes_read;

	buffer->len = 0;
	has_data = 0;
	bytes_read = read(STDIN_FILENO, &ch, 1);
	while (bytes_read > 0)
	{
		has_data = 1;
		if (ch == '\n')
			break ;
		if (!buffer_append(buffer, (unsigned char *)&ch, 1))
			return (-1);
		bytes_read = read(STDIN_FILENO, &ch, 1);
	}
	if (bytes_read < 0)
		return (-1);
	if (!has_data)
		return (0);
	if (!buffer_append(buffer, (unsigned char *)"\0", 1))
		return (-1);
	buffer->len--;
	return (1);
}

static int	execute_stdin_line(t_buffer *line, int *status)
{
	char	**argv;
	int		argc;

	argc = 0;
	argv = NULL;
	if (!tokenize_line((const char *)line->data, &argc, &argv))
	{
		print_system_error("ft_ssl", "stdin");
		return (0);
	}
	if (argc > 0 && execute_command_argv(argc, argv) != 0)
		*status = 1;
	free_argv_tokens(argv, argc);
	return (1);
}

int	run_stdin_commands(void)
{
	t_buffer	line;
	int			status;
	int			read_status;

	line.data = NULL;
	line.len = 0;
	line.capacity = 0;
	status = 0;
	read_status = read_command_line(&line);
	while (read_status > 0)
	{
		if (!execute_stdin_line(&line, &status))
		{
			buffer_free(&line);
			return (1);
		}
		read_status = read_command_line(&line);
	}
	if (read_status < 0)
	{
		print_system_error("ft_ssl", "stdin");
		status = 1;
	}
	buffer_free(&line);
	return (status);
}
