/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 22:46:16 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/03 18:32:34 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	expander(t_cmd_table **table)
{
	t_cmd_table		*current;
	size_t			c;

	current = *table;
	c = 0;
	while (current)
	{
		while ((current->args)[c])
		{
			if (c != 0)
			{
				if ((current->args)[c][0] != '\'')
				{
					if (expand_exit_status(&((current->args)[c])) == -1
						|| expand_env(&((current->args)[c])) == -1)
						return (-1);
				}
			}
			ft_trim_quotes(&((current->args)[c]));
			c++;
		}
		c = 0;
		if (current->args)
			current->cmd = (current->args)[0];
		if (current->heredoc_delim)
			ft_trim_quotes(&(current->heredoc_delim));
		if (current->input_redir)
			ft_trim_quotes(&(current->input_redir));
		if (current->output_redir)
			ft_trim_quotes(&(current->output_redir));
		if (current->append_redir)
			ft_trim_quotes(&(current->append_redir));
		current = current->next;
	}
	return (0);
}
