/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 21:55:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/16 21:55:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUM_H
# define ENUM_H

typedef enum e_command_group
{
	COMMAND_GROUP_STANDARD,
	COMMAND_GROUP_MESSAGE_DIGEST,
	COMMAND_GROUP_CIPHER
}	t_command_group;

typedef enum e_digest_source_kind
{
	DIGEST_SOURCE_STDIN_DEFAULT,
	DIGEST_SOURCE_STDIN_P,
	DIGEST_SOURCE_STRING,
	DIGEST_SOURCE_FILE
}	t_digest_source_kind;

#endif
