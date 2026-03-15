#include "ft_ssl.h"

const t_digest_algorithm	*get_md5_algorithm(void);
const t_digest_algorithm	*get_sha256_algorithm(void);
const t_digest_algorithm	*get_whirlpool_algorithm(void);

static const t_command	g_commands[] = {
	{"md5", COMMAND_GROUP_MESSAGE_DIGEST, run_digest_command, (const void *)get_md5_algorithm},
	{"sha256", COMMAND_GROUP_MESSAGE_DIGEST, run_digest_command, (const void *)get_sha256_algorithm},
	{"whirlpool", COMMAND_GROUP_MESSAGE_DIGEST, run_digest_command, (const void *)get_whirlpool_algorithm},
};

const t_command	*get_command_table(size_t *count)
{
	if (count != NULL)
		*count = sizeof(g_commands) / sizeof(g_commands[0]);
	return (g_commands);
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
