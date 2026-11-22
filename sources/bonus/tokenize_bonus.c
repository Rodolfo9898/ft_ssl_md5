/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:08:55 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 16:10:55 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_ssl_md5.h"

/* Only with -D BONUS_MODE=1 (make bonus): see include/core/defines.h */
#if BONUS_MODE

static int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r'
		|| c == '\v' || c == '\f');
}

static int	append_escaped(const char *line, t_split_state *state)
{
	state->index++;
	if (!split_append_char(state, line[state->index]))
		return (0);
	state->index++;
	return (1);
}

static int	parse_token(const char *line, t_split_state *state)
{
	state->len = 0;
	state->quote = 0;
	while (line[state->index]
		&& (state->quote || !is_space(line[state->index])))
	{
		if (!state->quote && (line[state->index] == '\''
				|| line[state->index] == '"'))
			state->quote = line[state->index++];
		else if (state->quote && line[state->index] == state->quote)
		{
			state->quote = 0;
			state->index++;
		}
		else if (line[state->index] == '\\' && line[state->index + 1])
		{
			if (!append_escaped(line, state))
				return (0);
		}
		else if (!split_append_char(state, line[state->index++]))
			return (0);
	}
	return (split_append_token(state));
}

/*
** A line of the command reader, to the words it stands for, as a shell would
** split it: spaces separate words, '...' and "..." group them, \ escapes the
** next character. argv_out is allocated: free it with free_argv_tokens().
*/
int	tokenize_line(const char *line, int *argc_out, char ***argv_out)
{
	t_split_state	state;

	ft_bzero(&state, sizeof(state));
	while (line[state.index] != '\0')
	{
		while (is_space(line[state.index]))
			state.index++;
		if (line[state.index] == '\0')
			break ;
		if (!parse_token(line, &state))
		{
			free(state.token);
			free_argv_tokens(state.argv, state.argc);
			return (0);
		}
	}
	free(state.token);
	*argc_out = state.argc;
	*argv_out = state.argv;
	return (1);
}

#endif
