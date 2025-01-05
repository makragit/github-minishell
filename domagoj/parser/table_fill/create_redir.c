/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 18:31:16 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/04 22:53:11 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

t_redir_data	*add_redir_entry(t_redir_data *data, t_token *token)
{
	t_redir_data	*new;
	t_token_type	type;

	if (!token)
		return (NULL);
	type = token->type;
	new = malloc(sizeof(t_redir_data));
	if (!new)
		return (NULL);
	ft_memset(new, 0, sizeof(t_redir_data));
	if (!new)
		return (NULL);
	if (type == APPEND)
		new->append = token->value;
	else if (type == INPUT)
		new->input = token->value;
	else if (type == OUTPUT)
		new->output = token->value;
	else if (type == HEREDOC)
		new->heredoc_delimiter = token->value;
	new->next = data;
	data = new;
	return (data);
}
