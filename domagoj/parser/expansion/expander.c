/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 22:46:16 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/10 16:42:28 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_err	expander(t_token *tokens)
{
	char	*current;

	while (tokens)
	{
		if (tokens->type != WORD && tokens->type != ARG_TOKEN  && tokens->type != CMD_t)
		{
			tokens = tokens->next;
			continue ;
		}
		current = ft_strdup(tokens->value);
		if (!current
			|| (ft_strncmp(current, "\'", 1) != S_QUOTE && tokens->type != PIPE
				&& (exit_status(&current) || expand_env(&current)))
			|| ft_trim_quotes(&current))
		{
			free(current);
			return (ERROR);
		}
		free(tokens->value);
		tokens->value = current;
		tokens = tokens->next;
	}
	return (OK);
}
