#include "ft_ssl.h"

int	run_help_command(const t_command *command, int argc, char **argv)
{
	(void)command;
	(void)argc;
	(void)argv;
	print_usage();
	print_command_list();
	return (1);
}
