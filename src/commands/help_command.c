/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 20:15:03 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/15 20:15:04 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
