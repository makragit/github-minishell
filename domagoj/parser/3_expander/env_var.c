/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 21:11:31 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/03 18:33:36 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	replace(char **arg)
{
	char	*env_var;
	char	*new_value;

	if (!arg || !*arg)
		return (-1);
	env_var = getenv(*arg);
	if (!env_var)
		env_var = "";
	new_value = ft_strdup(env_var);
	if (!new_value)
		return (-1);
	free(*arg);
	*arg = new_value;
	return (0);
}

int	expand_env(char **arg)
{
	char		*tmp;
	char		*new;
	char		*var_name;
	char		*joined;
	size_t		c;

	c = 0;
	tmp = *arg;
	if (!tmp)
		return (replace(arg));
	new = ft_strdup("");
	if (!new)
		return (-1);
	while (tmp[c])
	{
		if (tmp[c] == '$' && ft_isalnum(tmp[c + 1]))
		{
			size_t start = ++c;
			while (tmp[c] && (ft_isalnum(tmp[c]) || tmp[c] == '_'))
				c++;
			var_name = ft_substr(tmp, start, c - start);
			if (!var_name || replace(&var_name) == -1)
			{
				free(new);
				return (-1);
			}
			joined = ft_strjoin(new, var_name);
			free(var_name);
			free(new);
			if (!joined)
				return (-1);
			new = joined;
		}
		else
		{
			char ch[2] = {tmp[c], '\0'};
			joined = ft_strjoin(new, ch);
			free(new);
			if (!joined)
				return (-1);
			new = joined;
			c++;
		}
	}
	free(*arg);
	*arg = new;
	return (0);
}
