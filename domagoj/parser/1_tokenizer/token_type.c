/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: domagoj <domagoj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 16:33:47 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/03 18:23:10 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static token_type	define_redirection_type(t_token *tokens)
{
	char	*str;

	str = tokens->previous->value;
	if (ft_strlen(str) <= 1)
	{
		if (str[0] == '<')
			return (INPUT);
		return (OUTPUT);
	}
	if (str[0] == '<' && str[1] == '<')
		return (HEREDOC);
	else if (str[0] == '>' && str[1] == '>')
		return (APPEND);
	if (str[0] == '<')
		return (INPUT);
	else
		return (OUTPUT);
}

void	token_categorisation(t_token *tokens)
{
	while (tokens)
	{
		if (!tokens->previous)
		{
			if (tokens->type != REDIRECTION)
				tokens->type = CMD_TOKEN;
		}
		else if (tokens->previous->type == CMD_TOKEN && tokens->type == WORD)
			tokens->type = ARG_TOKEN;
		else if (tokens->previous->type == REDIRECTION)
		{
			tokens->type = define_redirection_type(tokens);
			free(tokens->previous->value);
			tokens = cut_token(tokens->previous);
		}
		else if (tokens->previous->type == PIPE_TOKEN)
			tokens->type = CMD_TOKEN;
		else if (tokens->previous->type == ARG_TOKEN && tokens->type == WORD)
			tokens->type = ARG_TOKEN;
		else if (tokens->type == WORD)
			tokens->type = ARG_TOKEN;
		tokens = tokens->next;
	}
}
