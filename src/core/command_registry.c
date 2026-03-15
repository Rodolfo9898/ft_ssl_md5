#include "ft_ssl.h"

static const t_command	g_commands[] = {
{"md5", COMMAND_GROUP_MESSAGE_DIGEST, run_digest_command, NULL},
{"sha256", COMMAND_GROUP_MESSAGE_DIGEST, run_digest_command, NULL},
};

const t_command	*get_command_table(size_t *count)
{
	if (count != NULL)
		*count = sizeof(g_commands) / sizeof(g_commands[0]);
	return (g_commands);
}

const t_command	*find_command(const char *name)
{
	size_t				count;
	size_t				index;
	const t_command		*commands;

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
