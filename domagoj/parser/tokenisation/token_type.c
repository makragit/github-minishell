/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 16:33:47 by domagoj           #+#    #+#             */
/*   Updated: 2025/02/01 17:12:19 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static t_token	*define_redirection_type(t_token *tokens)
{
	char	*str;

	str = tokens->value;
	if (ft_strlen(str) <= 1)
	{
		if (str[0] == '<')
			tokens->next->type = INPUT;
		tokens->next->type = OUTPUT;
	}
	if (str[0] == '<' && str[1] == '<')
		tokens->next->type = HEREDOC;
	else if (str[0] == '>' && str[1] == '>')
		tokens->next->type = APPEND;
	else if (str[0] == '<')
		tokens->next->type = INPUT;
	else if (str[0] == '>')
		tokens->next->type = OUTPUT;
	free(tokens->value);
	tokens = cut_token(tokens);
	return (tokens);
}

void	token_categorisation(t_token **head)
{
	t_token	*tokens;
	int		cmd_c;

	tokens = *head;
	cmd_c = 0;
	while (tokens)
	{
		if (tokens->type == PIPE)
			cmd_c = 0;
		else if (tokens->type == REDIRECTION)
		{
			if (!tokens->prev)
				*head = tokens->next;
			tokens = define_redirection_type(tokens);
		}
		if (cmd_c == 0 && tokens->type == WORD)
		{
			cmd_c = 1;
			tokens->type = CMD_t;
		}
		else if (tokens->type == WORD)
			tokens->type = ARG_TOKEN;
		tokens = tokens->next;
	}
}
