/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdin_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:06:17 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 16:10:55 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/* Only with -D BONUS_MODE=1 (make bonus): see include/core/defines.h */
#if BONUS_MODE

/*
** One byte per read(): the command reader must not take more than its line
** from the standard input, which a command with -p reads next. READER_CONTINUE:
** a line is in buffer, READER_STOP: end of the input, READER_ERROR: printed.
*/
static t_reader_status	read_command_line(t_buffer *buffer)
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
			return (print_system_error(NULL, "stdin"), READER_ERROR);
		bytes_read = read(STDIN_FILENO, &ch, 1);
	}
	if (bytes_read < 0)
		return (print_system_error(NULL, "stdin"), READER_ERROR);
	if (!has_data)
		return (READER_STOP);
	if (!buffer_append(buffer, (unsigned char *)"\0", 1))
		return (print_system_error(NULL, "stdin"), READER_ERROR);
	buffer->len--;
	return (READER_CONTINUE);
}

static int	is_exit_command(char **argv, int argc)
{
	return (argc > 0 && (ft_strncmp(argv[0], CMD_EXIT, sizeof(CMD_EXIT)) == 0
			|| ft_strncmp(argv[0], CMD_QUIT, sizeof(CMD_QUIT)) == 0));
}

/*
** Runs a line as if its words were the arguments of ft_ssl: an empty line
** does nothing. READER_STOP after exit or quit, READER_ERROR out of memory.
*/
static t_reader_status	execute_stdin_line(t_buffer *line, int *status)
{
	char			**argv;
	int				argc;
	t_reader_status	next;

	argc = 0;
	argv = NULL;
	if (!tokenize_line((const char *)line->data, &argc, &argv))
	{
		print_system_error(NULL, "stdin");
		return (READER_ERROR);
	}
	next = READER_CONTINUE;
	if (is_exit_command(argv, argc))
		next = READER_STOP;
	else if (argc > 0 && execute_command_argv(argc, argv) != EXIT_SUCCESS)
		*status = EXIT_FAILURE;
	free_argv_tokens(argv, argc);
	return (next);
}

/*
** The bonus: ft_ssl without a command reads one command per line until
** exit, quit or the end of the input. Status 1 when a command failed.
*/
int	run_stdin_commands(void)
{
	t_buffer		line;
	int				status;
	t_reader_status	result;

	ft_bzero(&line, sizeof(line));
	status = EXIT_SUCCESS;
	result = read_command_line(&line);
	while (result == READER_CONTINUE)
	{
		result = execute_stdin_line(&line, &status);
		if (result == READER_CONTINUE)
			result = read_command_line(&line);
	}
	buffer_free(&line);
	if (result == READER_ERROR)
		return (EXIT_FAILURE);
	return (status);
}

#endif
