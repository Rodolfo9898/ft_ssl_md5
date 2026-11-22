/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdin_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:06:17 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 18:05:35 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/* Only with -D BONUS_MODE=1 (make bonus): see include/core/defines.h */
#if BONUS_MODE

/* A failed read() or malloc while reading a command: say so, then stop */
static t_reader_status	reader_error(void)
{
	print_system_error(NULL, "stdin");
	return (READER_ERROR);
}

/*
** One byte per read(): the command reader must not take more than its line
** from the standard input, which a command with -p reads next. READER_CONTINUE:
** a line is in buffer, READER_STOP: the input is over, READER_ERROR: printed.
*/
static t_reader_status	read_command_line(t_buffer *buffer)
{
	char	ch;
	ssize_t	bytes_read;

	buffer->len = 0;
	bytes_read = read(STDIN_FILENO, &ch, 1);
	while (bytes_read > 0 && ch != '\n')
	{
		if (!buffer_append(buffer, (unsigned char *)&ch, 1))
			return (reader_error());
		bytes_read = read(STDIN_FILENO, &ch, 1);
	}
	if (bytes_read < 0)
		return (reader_error());
	if (bytes_read == 0 && buffer->len == 0)
		return (READER_STOP);
	if (!buffer_append(buffer, (unsigned char *)"\0", 1))
		return (reader_error());
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
		return (reader_error());
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
