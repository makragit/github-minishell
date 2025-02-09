/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:32:24 by mkrausho          #+#    #+#             */
/*   Updated: 2025/02/01 19:32:25 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// returns 0 on SUCCESS, errno or 1 on Failure
int	builtin_export(char **args)
{
	int	i;
	int	ret;

	ret = 1;
	if (mak_arr_size(args) == 1)
	{
		builtin_env(args);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		ret = export_handle_key_value(args, &i);
		if (args[i] != NULL)
			i++;
	}
	return (ret);
}

// returns 0 on SUCCESS, errno or 1 on Failure
int	builtin_unset(char **args)
{
	int	i;
	int	search;

	if (mak_arr_size(args) == 1)
		return (0);
	i = 1;
	while (args[i])
	{
		search = search_key_in_array(get_data(NULL)->mini_envp, args[i]);
		if (env_key_valid(args[i]) >= 2 || search < 1)
		{
			i++;
			continue ;
		}
		if (!array_free_and_rem(&get_data(NULL)->mini_envp, args[i]))
			return (1);
		i++;
	}
	return (0);
}

// returns malloced formatted key_value
char	*export_format_key_value(char **args, int *i, int ret)
{
	char	*key_value;

	key_value = NULL;
	if (ret == 1 || (ret == 2 && !args[*i + 1]))
		key_value = ft_strjoin(args[*i], "=\'\'");
	else if (ret == 2)
	{
		key_value = ft_strjoin(args[*i], args[*i + 1]);
		(*i)++;
	}
	else if (ret == 3)
		key_value = ft_strdup(args[*i]);
	return (key_value);
}

// 0 invalid
// 1 ends without =
// 2 ends with =
// 3 not ending with = (abc=test)
int	export_handle_key_value(char **args, int *i)
{
	int		ret;
	char	*key_value;

	ret = env_key_valid(args[*i]);
	if (ret == 0)
		return (bsh_err("export", args[*i], "not a valid identifier", 1));
	array_free_and_rem(&get_data(NULL)->mini_envp, args[*i]);
	key_value = export_format_key_value(args, i, ret);
	if (!key_value)
		malloc_error("ERROR: malloc fail expt_hndle_key strjoin");
	if (array_free_and_add(&get_data(NULL)->mini_envp, key_value) == -1)
	{
		free(key_value);
		malloc_error("exprt_hndle_key malloc array_free_and_add");
		return (0);
	}
	free(key_value);
	return (0);
}
