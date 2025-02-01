/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_array_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:32:04 by mkrausho          #+#    #+#             */
/*   Updated: 2025/02/01 19:32:05 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Takes malloced array, frees it and creates new array 
 * with added Value using the same arr ptr*/
/* 0 on fail 1 success ; -1 on malloc error (to fix a free-leak */
int	array_free_and_add(char ***arr, char *new_value)
{
	char	**new_arr;
	int		size;

	if (!arr || !*arr)
		return (0);
	if (!new_value)
		return (0);
	size = mak_arr_size(*arr);
	new_arr = malloc(sizeof(char *) * (size + 2));
	if (!new_arr)
		return (-1);
	if (array_add_help(*arr, new_arr, new_value) == -1)
		return (-1);
	mak_free_array(*arr);
	*arr = new_arr;
	return (1);
}

int	array_add_help(char **arr, char **new_arr, char *new_v)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		new_arr[i] = ft_strdup(arr[i]);
		if (!new_arr[i])
		{
			mak_free_array(new_arr);
			return (-1);
		}
		i++;
	}
	new_arr[i] = ft_strdup(new_v);
	if (!new_arr[i])
	{
		mak_free_array(new_arr);
		return (-1);
	}
	new_arr[++i] = NULL;
	return (0);
}

// FIX: Searches remove_value Key, 
// removes all instances of remove_value and creates new array
int	array_free_and_rem(char ***arr, char *rem_value)
{
	char	**new_arr;
	int		found;
	char	**temp_arr;

	temp_arr = (*arr);
	if (!arr || !*arr || !rem_value)
		return (0);
	found = search_key_in_array((*arr), rem_value);
	if (!search_key_in_array((*arr), rem_value))
		return (0);
	new_arr = malloc(sizeof(char *) * (mak_arr_size(*arr) - found + 1));
	if (!new_arr)
		malloc_error("ERROR: malloc failed in array_free_and_rem");
	new_arr = array_rem_help(temp_arr, new_arr, rem_value);
	mak_free_array(*arr);
	*arr = new_arr;
	return (1);
}

char	**array_rem_help(char **arr, char **new_arr, char *rem)
{
	int	i;

	i = 0;
	while (*arr)
	{
		if (ft_strncmp(*arr, rem, len_to_equal_sign(rem)) != 0)
		{
			new_arr[i] = ft_strdup(*arr);
			if (!new_arr[i++])
			{
				mak_free_array(new_arr);
				malloc_error("ERROR: malloc fail in arr_rem_hlp: ft_strdp");
			}
		}
		arr++;
	}
	new_arr[i] = NULL;
	return (new_arr);
}
