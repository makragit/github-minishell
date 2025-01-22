/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 21:11:31 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/22 14:20:25 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static char	*fetch_ex_cde(void)
{
	int		ex_code;
	char	*return_str;
	t_data	*data;

	data = get_data(NULL);
	ex_code = data->last_ex_code;
	return_str = ft_itoa(ex_code);
	return (return_str);
}

static int	replace(char **arg, char *var_name, size_t start)
{
	char	*env_var;
	char	*new_value;
	int		len;

	if (!var_name || !*arg || !arg)
		return (-1);
	if (var_name[1] == '?')
	{
		env_var = fetch_ex_cde();
		new_value = env_var;
	}
	else
	{
		env_var = getenv_local(var_name + 1);
		if (!env_var)
		{
			if (!ft_strncmp("$", var_name, ft_strlen(var_name)))
				env_var = "$";
			else
				env_var = "";
		}
		new_value = ft_strdup(env_var);
	}
	len = ft_strlen(new_value);
	free(var_name);
	if (!new_value)
		return (-1);
	if (ft_str_insert(arg, new_value, start) == -1)
	{
		free(new_value);
		return (-1);
	}
	free(new_value);
	return (len);
}

static char	*_var_name(char **str, size_t start, size_t len)
{
	char	*tmp;
	char	*part;

	part = ft_substr(*str, start, len);
	if (!part)
		return (NULL);
	tmp = ft_strexpel(*str, part, ONE);
	if (!tmp)
	{
		free(part);
		return (NULL);
	}
	free(*str);
	*str = tmp;
	return (part);
}

static size_t	get_len(char *str)
{
	size_t	i;

	i = 1;
	while (str[i] && ft_isalnum(str[i]))
		i++;
	if (i == 1 && str[i] == '?')
		i = 2;
	return (i);
}

int	expand_env(char **arg)
{
	char		*str_cpy;
	char		*var_name;
	size_t		c;
	int			len;

	str_cpy = ft_strdup(*arg);
	if (!str_cpy)
		return (1);
	c = 0;
	while (str_cpy[c])
	{
		if (str_cpy[c] == '\'')
		{
			c = ft_strchr(str_cpy + c + 1, '\'') - str_cpy + 1;
			if ((int)c > ft_strlen(str_cpy))
				break ;
			continue ;
		}
		if (str_cpy[c] == '$')
		{
			len = get_len(str_cpy + c);
			var_name = _var_name(&str_cpy, c, len);
			if (!var_name)
			{
				free(str_cpy);
				return (-1);
			}
			len = replace(&str_cpy, var_name, c);
			if (len == -1)
			{
				free(str_cpy);
				return (-1);
			}
			c += len;
		}
		else
			c++;
	}
	free(*arg);
	*arg = str_cpy;
	return (0);
}
