/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strexpel.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 13:06:52 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/03 18:25:57 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*cut_connect(char *str, size_t index, size_t cut_len)
{
	char	*part1;
	char	*part2;
	char	*return_str;

	part1 = ft_substr(str, 0, index);
	if (!part1)
	{
		free(str);
		return (NULL);
	}
	part2 = ft_strdup(str + index + cut_len);
	if (!part2)
	{
		free(str);
		free(part1);
		return (NULL);
	}
	return_str = ft_strjoin(part1, part2);
	free(part1);
	free(part2);
	free(str);
	return (return_str);
}

char	*ft_strexpel(char *str, const char *expel)
{
	size_t	len1;
	size_t	len2;
	char	*result;
	char	*str_cpy;

	if (!str || !expel)
		return (NULL);
	str_cpy = ft_strdup(str);
	if (!str_cpy)
		return (NULL);
	len1 = ft_strlen(str_cpy);
	len2 = ft_strlen(expel);
	if (len2 < 1 || len1 < 1)
		return (ft_strdup(str));
	result = ft_strnstr(str_cpy, expel, len1);
	if (!result)
		return (str);
	str_cpy = cut_connect(str_cpy, result - str_cpy, len2);
	if (!str_cpy)
		return (NULL);
	len1 = ft_strlen(str_cpy);
	return (str_cpy);
}
