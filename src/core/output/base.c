/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 20:14:22 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/15 20:14:23 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	ft_putstr_fd(int fd, const char *str)
{
	write_all(fd, str, ft_strlen(str));
}

void	ft_putendl_fd(int fd, const char *str)
{
	ft_putstr_fd(fd, str);
	write_all(fd, "\n", 1);
}

void	ft_puthex_fd(int fd, const unsigned char *data, size_t len)
{
	static const char	hex[] = "0123456789abcdef";
	char				out[2];
	size_t				index;

	index = 0;
	while (index < len)
	{
		out[0] = hex[data[index] >> 4];
		out[1] = hex[data[index] & 0x0F];
		write_all(fd, out, 2);
		index++;
	}
}

void	print_usage(void)
{
	ft_putendl_fd(2, "usage: ft_ssl command [flags] [file/string]");
}

void	print_invalid_command(const char *name)
{
	ft_putstr_fd(2, "ft_ssl: Error: '");
	ft_putstr_fd(2, name);
	ft_putendl_fd(2, "' is an invalid command.");
	print_command_list();
}
