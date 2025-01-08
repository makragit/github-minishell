/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 22:46:16 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/07 20:22:18 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

t_err	expander(t_token *tokens)
{
	char		*current;
	t_token		*head;

	head = tokens;
	while (tokens)
	{
		current = ft_strdup(tokens->value);
		if (!current)
			return (ERROR);
		if (ft_strncmp(current, "\'", 1) != S_QUOTE && tokens->type != PIPE_TOKEN)
		{
			if (expand_exit_status(&current))
			{
				free(current);
				return (ERROR);
			}
			if (expand_env(&current))
			{
				free(current);
				return (ERROR);
			}
		}
		if (ft_trim_quotes(&current))
		{
			free(current);
			return (ERROR);
		}
		free(tokens->value);
		tokens->value = current;
		tokens = tokens->next;
	}
	tokens = head;
	return (OK);
}
