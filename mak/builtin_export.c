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

// Function to sort and print an array of strings without modifying the original
void	builtin_export_print(char **arr, int size)
{
	char	**temp;
	int		i;
	char	*key_end;
	int		key_length;

	temp = malloc(size * sizeof(char *));
	if (!temp)
		malloc_error("builtin_export_print malloc error");
	i = 0;
	while (i < size)
	{
		temp[i] = arr[i];
		i++;
	}
	mak_arr_sort(temp, size);
	i = 0;
	while (i < size)
	{
		key_end = ft_strchr(temp[i], '=');
		key_length = key_end - temp[i];
		printf("declare -x %.*s=\"%s\"\n", key_length, temp[i], key_end + 1);
		i++;
	}
	free(temp);
}

// returns 0 on SUCCESS, errno or 1 on Failure
int	builtin_export(char **args)
{
	int		i;
	int		ret;
	char	**get_envp;

	ret = 1;
	if (mak_arr_size(args) == 1)
	{
		get_envp = get_data(NULL)->mini_envp;
		builtin_export_print(get_envp, mak_arr_size(get_envp));
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

int	export_update_no_equal(char *arg)
{
	char	*temp;
	char	*value;

	if (search_key_in_array(get_data(NULL)->mini_envp, arg) > 0)
		return (0);
	temp = ft_strjoin(arg, "=");
	if (!temp)
		malloc_error("export_handle_key_value ft_strjoin malloc error");
	value = "";
	update_env_var(temp, value);
	free(temp);
	return (0);
}

int	export_update_with_value(char *arg)
{
	char	*temp;
	char	*value;
	int		len_to;

	len_to = len_to_equal_sign(arg);
	temp = ft_substr(arg, 0, len_to + 1);
	if (!temp)
		malloc_error("export_handle_key_value ft_substr malloc error");
	value = arg + len_to + 1;
	update_env_var(temp, value);
	free(temp);
	return (0);
}

int	export_handle_key_value(char **args, int *i)
{
	int		ret;
	char	*value;

	value = NULL;
	ret = env_key_valid(args[*i]);
	if (ret == 0)
		return (bsh_err("export", args[*i], "not a valid identifier", 1));
	if (ret == 1)
		return (export_update_no_equal(args[*i]));
	/* else if (ret == 2 && args[*i + 1] == NULL) */
	/* 	value = ""; */
	/* else if (ret == 2) */
	/* { */
	/* 	update_env_var(args[*i], args[*i + 1]); */
	/* 	(*i)++; */
	/* 	return (0); */
	/* } */

	else if (ret == 2)
		value = "";




	else if (ret == 3)
		return (export_update_with_value(args[*i]));
	update_env_var(args[*i], value);
	return (0);
}
