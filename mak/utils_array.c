/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_array.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:32:00 by mkrausho          #+#    #+#             */
/*   Updated: 2025/02/01 19:32:01 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	mak_arr_size(char **arr)
{
	int	i;

	if (!arr)
		return (0);
	i = 0;
	while (arr[i])
		i++;
	return (i);
}

char	**copy_array(char **arr)
{
	char	**new_arr;
	int		size;
	int		i;

	if (!arr || !*arr)
		return (NULL);
	size = mak_arr_size(arr);
	new_arr = malloc(sizeof(char *) * (size + 1));
	if (!new_arr)
		malloc_error("ERROR: malloc failed in copy_array");
	i = 0;
	while (arr[i])
	{
		new_arr[i] = ft_strdup(arr[i]);
		if (!new_arr[i])
		{
			mak_free_array(new_arr);
			malloc_error("ERROR malloc failed in copy_array: ft_strdup");
		}
		i++;
	}
	new_arr[i] = NULL;
	return (new_arr);
}
