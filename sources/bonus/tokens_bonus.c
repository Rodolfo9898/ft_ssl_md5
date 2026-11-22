/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:08:39 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 16:10:55 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/*
** The words of tokenize_bonus.c: the one being built, and the array of them.
** Only with -D BONUS_MODE=1 (make bonus).
*/
#if BONUS_MODE

static int	grow_token(t_split_state *state)
{
	char	*new_buffer;
	size_t	new_capacity;

	new_capacity = state->capacity;
	if (new_capacity == 0)
		new_capacity = TOKEN_FIRST_SIZE;
	while (state->len + 1 >= new_capacity)
		new_capacity *= GROWTH_FACTOR;
	new_buffer = (char *)malloc(new_capacity);
	if (new_buffer == NULL)
		return (0);
	if (state->token != NULL)
		ft_memcpy(new_buffer, state->token, state->len);
	free(state->token);
	state->token = new_buffer;
	state->capacity = new_capacity;
	return (1);
}

int	split_append_char(t_split_state *state, char c)
{
	if (state->len + 1 >= state->capacity && !grow_token(state))
		return (0);
	state->token[state->len] = c;
	state->len++;
	return (1);
}

static int	grow_argv(t_split_state *state)
{
	char	**new_argv;
	int		new_capacity;

	new_capacity = state->argv_capacity;
	if (new_capacity == 0)
		new_capacity = ARGV_FIRST_SIZE;
	while (state->argc >= new_capacity)
		new_capacity *= GROWTH_FACTOR;
	new_argv = (char **)malloc(sizeof(char *) * new_capacity);
	if (new_argv == NULL)
		return (0);
	if (state->argv != NULL)
		ft_memcpy(new_argv, state->argv, sizeof(char *) * state->argc);
	free(state->argv);
	state->argv = new_argv;
	state->argv_capacity = new_capacity;
	return (1);
}

int	split_append_token(t_split_state *state)
{
	char	*copy;

	copy = (char *)malloc(state->len + 1);
	if (copy == NULL)
		return (0);
	ft_memcpy(copy, state->token, state->len);
	copy[state->len] = '\0';
	if (state->argc >= state->argv_capacity && !grow_argv(state))
	{
		free(copy);
		return (0);
	}
	state->argv[state->argc] = copy;
	state->argc++;
	return (1);
}

void	free_argv_tokens(char **argv, int argc)
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

#endif
