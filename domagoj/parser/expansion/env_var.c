/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 21:11:31 by domagoj           #+#    #+#             */
/*   Updated: 2025/02/14 20:01:53 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static size_t	of_len(char *str)
{
	size_t	i;

	i = 1;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (i == 1 && str[i] == '?')
		i = 2;
	return (i);
}

static int	decide_q_flag(int d_q)
{
	if (d_q == 1)
		return (0);
	else
		return (1);
}

static int	skip_heredoc(char *str, int c)
{
	if (ft_strlen(str + c) > 2)
	{
		if (str[c] == '<' && str[c + 1] == '<')
		{
			c += 2;
			if (str[c])
			{
				while (str[c] && is_whitespace(str[c]))
					c++;
				while (str[c]
					&& (ft_isalpha(str[c]) || str[c] == '_' || str[c] == '$'))
					c++;
				return (c);
			}
		}
		return (c);
	}
	return (c);
}

int	expand_env(char **arg, int d_q, int c, char *cpy)
{
	char	*var_name;

	if (!cpy)
		return (1);
	while (cpy[c])
	{
		if (cpy[c] == '\"')
			d_q = decide_q_flag(d_q);
		if (skip_s_quote(cpy, &c, d_q) == -1)
			break ;
		c = skip_heredoc(cpy, c);
		if (cpy[c] == '$')
		{
			var_name = _var_name(&cpy, c, of_len(cpy + c));
			c += replace(&cpy, var_name, c);
			if (c < 0)
				break ;
		}
		else if (cpy[c])
			c++;
	}
	free(*arg);
	*arg = cpy;
	return (0);
}
