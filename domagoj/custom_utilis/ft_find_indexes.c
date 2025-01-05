/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find_indexes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 17:51:15 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/04 22:52:07 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	*find_indexes(char *str, char *needle)
{
	int		*array;
	char	*str_cpy;
	char	*match;
	size_t	c;
	size_t	i;

	str_cpy = str;
	c = 0;
	i = 0;
	if (!str || !needle || ft_strlen(needle) == 0)
		return (NULL);
	while (str)
	{
		str = ft_strnstr(str, needle, ft_strlen(str));
		if (!str)
			break ;
		c++;
		str += ft_strlen(needle);
	}
	if (c == 0)
		return (NULL);
	array = malloc(sizeof(int) * (c + 1));
	if (!array)
		return (NULL);
	array[c] = -1;
	str = str_cpy;
	while (i < c)
	{
		match = ft_strnstr(str, needle, ft_strlen(str));
		array[i] = match - str_cpy;
		str = match + ft_strlen(needle);
		i++;
	}
	return (array);
}
