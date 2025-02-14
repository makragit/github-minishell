/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 19:41:22 by dbogovic          #+#    #+#             */
/*   Updated: 2025/02/14 19:51:38 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

int	replace(char **arg, char *var_name, size_t start)
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
