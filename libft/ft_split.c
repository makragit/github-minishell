/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:10:41 by mkrausho          #+#    #+#             */
/*   Updated: 2024/10/15 19:24:56 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Name						ft_split
Prototyp				char **ft_split(char const *s, char c);
-
Parameters			s: The string to be split.
								c: The delimiter character.
Return					The array of new strings resulting from the split.
								NULL if the allocation fails.
external				malloc, free
Description			Allocates (with malloc(3)) and returns an array
								of strings obtained by splitting ’s’ using the
								character ’c’ as a delimiter. The array must end
								with a NULL pointer.
*/

#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

// DEL
#include <stdio.h>

static int	free_array(char **arr, size_t j);
static int	fill_array(char const *s, char c, char **arr);

char	**ft_split(char const *s, char c)
{
	char			**arr;
	unsigned int	i;
	unsigned int	substrings;

	if (!s)
		return (NULL);
	substrings = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c)
			substrings++;
		while (s[i] && s[i] != c)
			i++;
		if (s[i])
			i++;
	}
	arr = (char **)malloc(sizeof(char *) * (substrings + 1));
	if (!arr)
		return (NULL);
	if (fill_array(s, c, arr) == 0)
		return (NULL);
	return (arr);
}

static int	fill_array(char const *s, char c, char **arr)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	start;

	j = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			start = i;
			while (s[i] && s[i] != c)
				i++;
			arr[j] = ft_substr(s, start, i - start);
			if (!arr[j++])
				return (free_array(arr, j));
		}
		if (s[i])
			i++;
	}
	arr[j] = (NULL);
	return (1);
}

static int	free_array(char **arr, size_t j)
{
	size_t	i;

	i = 0;
	while (i < j)
	{
		free(arr[i]);
		arr[i] = (NULL);
		i++;
	}
	free(arr);
	arr = (NULL);
	return (0);
}
