/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_make_entry.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 16:07:17 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/03 18:22:33 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	make_token_entry(t_token **tokens, char *str_token)
{
	t_token	*new;
	if (!str_token)
		return (-1);
	new = malloc(sizeof(t_token));
	if (!new)
		return (-1);
	new->previous = NULL;
	new->next = NULL;
	new->next = (*tokens);
	if (str_token[0] == '|')
		new->type = PIPE_TOKEN;
	else if (str_token[0] == '<' || str_token[0] == '>')
		new->type = REDIRECTION;
	else
		new->type = WORD;
	new->value = str_token;
	(*tokens) = new;
	return (0);
}
