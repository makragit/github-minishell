/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 22:46:16 by domagoj           #+#    #+#             */
/*   Updated: 2025/02/11 15:44:01 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static int	check_quoted(char *str)
{
	if (ft_strchr(str, '\''))
		return (1);
	if (ft_strchr(str, '\"'))
		return (1);
	return (0);
}

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
		if (tokens->type == HEREDOC)
			tokens->is_quoted = check_quoted(tokens->value);
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
