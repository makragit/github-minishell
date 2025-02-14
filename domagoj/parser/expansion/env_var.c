/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 21:11:31 by domagoj           #+#    #+#             */
/*   Updated: 2025/02/14 19:04:20 by mkrausho         ###   ########.fr       */
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

static int skip_heredoc(char *str, int c)
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
				while (str[c] && (ft_isalpha(str[c]) || str[c] == '_' || str[c] == '$'))
					c++;
				return (c);
			}
		}
		return (c);
	}

	return (c);

}

int	expand_env(char **arg, int d_q, int c)
{
	char	*str_cpy;
	char	*var_name;

	str_cpy = ft_strdup(*arg);
	if (!str_cpy)
		return (1);
	while (str_cpy[c])
	{
		if (str_cpy[c] == '\"')
			d_q = decide_q_flag(d_q);
		if (skip_s_quote(str_cpy, &c, d_q) == -1)
			break ;
		c = skip_heredoc(str_cpy, c);
		if (str_cpy[c] == '\0')
			break;
		if (str_cpy[c] == '$')
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
