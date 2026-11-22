/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:08:55 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 18:05:35 by rperez-t         ###   ########.fr       */
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

/* \ and the character after it: the character is kept as it is */
static int	append_escaped(const char *line, t_split_state *state)
{
	state->index++;
	if (!split_append_char(state, line[state->index]))
		return (0);
	state->index++;
	return (1);
}

/*
** One character of a word: a quote opens or closes a quoted part, \ escapes
** the next character, anything else is kept. 0 when malloc failed.
*/
static int	parse_char(const char *line, t_split_state *state)
{
	char	c;

	c = line[state->index];
	if (c == '\\' && line[state->index + 1] != '\0')
		return (append_escaped(line, state));
	if (state->quote == 0 && (c == '\'' || c == '"'))
		state->quote = c;
	else if (state->quote != 0 && c == state->quote)
		state->quote = 0;
	else if (!split_append_char(state, c))
		return (0);
	state->index++;
	return (1);
}

/* One word, up to a space outside quotes or the end of the line */
static int	parse_token(const char *line, t_split_state *state)
{
	state->len = 0;
	state->quote = 0;
	while (line[state->index] != '\0'
		&& (state->quote != 0 || !is_space(line[state->index])))
	{
		if (!parse_char(line, state))
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
