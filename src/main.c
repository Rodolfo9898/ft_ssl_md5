#include "ft_ssl.h"
#include <stdlib.h>
#include <unistd.h>

static int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r'
		|| c == '\v' || c == '\f');
}

static int	append_char(char **buffer, size_t *len, size_t *capacity, char c)
{
	char	*new_buffer;
	size_t	new_capacity;

	if (*len + 1 < *capacity)
	{
		(*buffer)[*len] = c;
		(*len)++;
		return (1);
	}
	new_capacity = *capacity;
	if (new_capacity == 0)
		new_capacity = 32;
	while (*len + 1 >= new_capacity)
		new_capacity *= 2;
	new_buffer = (char *)malloc(new_capacity);
	if (new_buffer == NULL)
		return (0);
	if (*buffer != NULL)
	{
		ft_memcpy(new_buffer, *buffer, *len);
		free(*buffer);
	}
	*buffer = new_buffer;
	*capacity = new_capacity;
	(*buffer)[*len] = c;
	(*len)++;
	return (1);
}

static int	append_token(char ***argv, int *argc, int *capacity,
	char *token, size_t token_len)
{
	char	**new_argv;
	char	*copy;
	int		new_capacity;

	copy = (char *)malloc(token_len + 1);
	if (copy == NULL)
		return (0);
	ft_memcpy(copy, token, token_len);
	copy[token_len] = '\0';
	if (*argc < *capacity)
	{
		(*argv)[*argc] = copy;
		(*argc)++;
		return (1);
	}
	new_capacity = *capacity;
	if (new_capacity == 0)
		new_capacity = 8;
	while (*argc >= new_capacity)
		new_capacity *= 2;
	new_argv = (char **)malloc(sizeof(char *) * new_capacity);
	if (new_argv == NULL)
	{
		free(copy);
		return (0);
	}
	if (*argv != NULL)
	{
		ft_memcpy(new_argv, *argv, sizeof(char *) * (*argc));
		free(*argv);
	}
	*argv = new_argv;
	*capacity = new_capacity;
	(*argv)[*argc] = copy;
	(*argc)++;
	return (1);
}

static void	free_argv_tokens(char **argv, int argc)
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

static int	tokenize_line(const char *line, int *argc_out, char ***argv_out)
{
	char	**argv;
	char	*token;
	size_t	index;
	size_t	len;
	size_t	capacity;
	int		argc;
	int		argv_capacity;
	char	quote;

	argv = NULL;
	token = NULL;
	index = 0;
	len = 0;
	capacity = 0;
	argc = 0;
	argv_capacity = 0;
	while (line[index] != '\0')
	{
		while (is_space(line[index]))
			index++;
		if (line[index] == '\0')
			break ;
		len = 0;
		quote = 0;
		while (line[index] != '\0' && (quote != 0 || !is_space(line[index])))
		{
			if (quote == 0 && (line[index] == '\'' || line[index] == '"'))
				quote = line[index++];
			else if (quote != 0 && line[index] == quote)
			{
				quote = 0;
				index++;
			}
			else if (line[index] == '\\' && line[index + 1] != '\0')
			{
				index++;
				if (!append_char(&token, &len, &capacity, line[index++]))
					goto error;
			}
			else if (!append_char(&token, &len, &capacity, line[index++]))
				goto error;
		}
		if (!append_token(&argv, &argc, &argv_capacity, token, len))
			goto error;
	}
	free(token);
	*argc_out = argc;
	*argv_out = argv;
	return (1);
error:
	free(token);
	free_argv_tokens(argv, argc);
	return (0);
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

static int	run_stdin_commands(void)
{
	t_buffer	line;
	char		**argv;
	int			argc;
	int			status;
	int			read_status;

	line.data = NULL;
	line.len = 0;
	line.capacity = 0;
	status = 0;
	read_status = read_command_line(&line);
	while (read_status > 0)
	{
		argc = 0;
		argv = NULL;
		if (!tokenize_line((const char *)line.data, &argc, &argv))
		{
			print_system_error("ft_ssl", "stdin");
			buffer_free(&line);
			return (1);
		}
		if (argc > 0 && execute_command_argv(argc, argv) != 0)
			status = 1;
		free_argv_tokens(argv, argc);
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

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (run_stdin_commands());
	return (execute_command_argv(argc - 1, argv + 1));
}
