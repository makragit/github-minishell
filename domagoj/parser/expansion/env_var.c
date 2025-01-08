/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 21:11:31 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/07 20:08:26 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

char	*getenv_local(char *name)
{
	char		**env;
	size_t		line;
	t_data		*shared_data;

	line = 0;
	shared_data = get_data(NULL);
	if (!shared_data)
		return (NULL);
	if (!shared_data->mini_envp)
		return (NULL);
	env = shared_data->mini_envp;
	while (env[line])
	{
		if (ft_strncmp(name, env[line], ft_strlen(name)) == OK)
			return (env[line] + (ft_strlen(name) + 1));
		line++;
	}
	return (NULL);
}

static int	replace(char **arg, char *var_name, size_t start)
{
	char	*env_var;
	char	*new_value;

	if (!arg || !*arg)
		return (1);
	env_var = getenv_local(var_name + 1);
	if (!env_var)
		env_var = "";
	new_value = ft_strdup(env_var);
	free(var_name);
	if (!new_value)
		return (1);
	if (ft_str_insert(arg, new_value, start) == -1)
	{
		free(new_value);
		return (1);
	}
	free(new_value);
	if (!arg)
		return (1);
	return (0);
}

static int	start_len(size_t *start, size_t *len, char *str)
{
	int		c;

	c = 0;
	if (!str)
		return (0);
	*len = 1;
	while (str[c])
	{
		if (str[c] == '$' && ft_isalnum(str[c + 1]))
		{
			*start = c++;
			while (str[c] && (ft_isalnum(str[c]) || str[c] == '_'))
			{
				(*len)++;
				c++;
			}
			return (1);
		}
		c++;
	}
	return (0);
}

static char	*extract(char **str, size_t start, size_t len)
{
	char	*tmp;
	char	*part;

	part = ft_substr(*str, start, len);
	if (!part)
		return (NULL);
	tmp = ft_strexpel(*str, part);
	if (!tmp)
	{
		free(part);
		return (NULL);
	}
	free(*str);
	*str = tmp;
	return (part);
}

int	expand_env(char **arg)
{
	char		*str_cpy;
	char		*var_name;
	size_t		start;
	size_t		len;

	str_cpy = ft_strdup(*arg);
	if (!str_cpy)
		return (1);
	while (start_len(&start, &len, str_cpy))
	{
		var_name = extract(&str_cpy, start, len);
		if (!var_name)
		{
			free(str_cpy);
			return (1);
		}
		if (replace(&str_cpy, var_name, start))
		{
			free(str_cpy);
			return (1);
		}
	}
	if (!str_cpy)
		return (1);
	free(*arg);
	*arg = str_cpy;
	return (0);
}
