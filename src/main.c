#include "ft_ssl.h"

int	main(int argc, char **argv)
{
	const t_command	*command;

	if (argc < 2)
	{
		print_usage();
		return (1);
	}
	command = find_command(argv[1]);
	if (command == NULL)
	{
		print_invalid_command(argv[1]);
		return (1);
	}
	return (command->handler(command, argc - 2, argv + 2));
}
