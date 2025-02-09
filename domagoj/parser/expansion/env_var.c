/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: domagoj <domagoj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 21:11:31 by domagoj           #+#    #+#             */
/*   Updated: 2025/02/06 12:24:14 by domagoj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static char	*get_expanded(char *var_name)
{
	char	*return_str;
	char	*envar;

	if (var_name[1] == '?')
		return_str = ft_itoa (get_data(NULL)->last_ex_code);
	else
	{
		envar = getenv_local(var_name + 1);
		if (!envar)
		{
			if (!ft_strncmp("$", var_name, ft_strlen(var_name)))
				envar = "$";
			else
				envar = "";
		}
		return_str = ft_strdup(envar);
	}
	return (return_str);
}

static int	replace(char **arg, char *var_name, size_t start)
{
	char	*new_value;
	int		len;

	if (!var_name || !*arg || !arg)
		return (-100000);
	new_value = get_expanded(var_name);
	if (!new_value)
	{
		free(var_name);
		return (-100000);
	}
	len = ft_strlen(new_value);
	free(var_name);
	if (ft_str_insert(arg, new_value, start) == -1)
	{
		free(new_value);
		return (-100000);
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

int	expand_env(char **arg)
{
	char	*str_cpy;
	char	*var_name;
	int		c;
	int		d_q;

	str_cpy = ft_strdup(*arg);
	if (!str_cpy)
		return (1);
	c = 0;
	d_q = 0;
	while (str_cpy[c])
	{
		if (str_cpy[c] == '\"')
		{
			if (d_q == 1)
				d_q = 0;
			else
				d_q = 1;
		}
		if (skip_s_quote(str_cpy, &c, d_q) == -1)
			break ;
		else if (str_cpy[c] == '$')
		{
			var_name = _var_name(&str_cpy, c, of_len(str_cpy + c));
			c += replace(&str_cpy, var_name, c);
			if (c < 0)
				break ;
		}
		else
			c++;
	}
	free(*arg);
	*arg = str_cpy;
	return (0);
}
