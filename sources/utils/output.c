/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:06:10 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 16:10:55 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/*
** The results go to the standard output through these two, which say when a
** write failed (a full disk, a closed output): the command then fails. The
** error messages go to the standard error with libft's ft_putstr_fd.
*/
int	write_all(int fd, const void *data, size_t len)
{
	const unsigned char	*bytes;
	ssize_t				written;

	bytes = (const unsigned char *)data;
	while (len > 0)
	{
		written = write(fd, bytes, len);
		if (written < 0)
			return (0);
		bytes += written;
		len -= (size_t)written;
	}
	return (1);
}

int	write_str(int fd, const char *str)
{
	return (write_all(fd, str, ft_strlen(str)));
}

/* "ft_ssl: command: target: reason", the parts that are given, from errno */
void	print_system_error(char *command, char *target)
{
	ft_putstr_fd("ft_ssl: ", STDERR_FILENO);
	if (command != NULL && command[0] != '\0')
	{
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (target != NULL && target[0] != '\0')
	{
		ft_putstr_fd(target, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
}
