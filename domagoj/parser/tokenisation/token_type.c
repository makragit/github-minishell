/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 16:33:47 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/04 22:53:33 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static t_token_type	define_redirection_type(t_token *tokens)
{
	char	*str;

	str = tokens->value;
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

void	token_categorisation(t_token **head)
{
	t_token	*tokens;

	tokens = *head;
	while (tokens)
	{
		if (!tokens->prev && tokens->type != REDIRECTION)
			tokens->type = CMD_t;
		else if (tokens->type == REDIRECTION)
		{
			if (!tokens->prev)
				*head = tokens->next;
			tokens->next->type = define_redirection_type(tokens);
			free(tokens->value);
			tokens = cut_token(tokens);
		}
		else if (tokens->prev)
		{
			if (tokens->type == WORD && tokens->prev->type != CMD_t && tokens->prev->type != ARG_TOKEN)
				tokens->type = CMD_t;
			else if (tokens->type == WORD)
				tokens->type = ARG_TOKEN;
		}
		tokens = tokens->next;
	}
	tokens = *head;
}
