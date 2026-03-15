/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 20:14:16 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/15 20:14:17 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (run_stdin_commands());
	return (execute_command_argv(argc - 1, argv + 1));
}
