/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 22:46:16 by domagoj           #+#    #+#             */
/*   Updated: 2025/02/02 14:56:32 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

t_err	expander(t_token *tokens)
{
	char	*current;

	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			tokens = tokens->next;
			continue ;
		}
		current = ft_strdup(tokens->value);
		if (!current || ft_trim_quotes(&current))
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
