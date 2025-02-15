/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strexpel.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 13:06:52 by domagoj           #+#    #+#             */
/*   Updated: 2025/02/15 14:06:03 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	cut_connect(char **str, size_t index, size_t cut_len)
{
	char	*part1;
	char	*part2;
	char	*tmp;

	part2 = NULL;
	if (!(*str))
		return ;
	if ((size_t)ft_strlen(*str) < index + cut_len)
		return ;
	part1 = ft_substr(*str, 0, index);
	if (part1)
	{
		part2 = ft_strdup(*str + index + cut_len);
		if (!part2)
		{
			free(part1);
			part1 = NULL;
		}
	}
	tmp = ft_strjoin(part1, part2);
	free(part1);
	free(part2);
	free(*str);
	*str = tmp;
}

static char	*expel_single(char *str, const char *expel)
{
	size_t	len1;
	size_t	len2;
	char	*result;
	char	*str_cpy;

	if (!expel)
		return (NULL);
	str_cpy = ft_strdup(str);
	if (!str_cpy)
		return (NULL);
	len1 = ft_strlen(str_cpy);
	len2 = ft_strlen(expel);
	if (len2 < 1 || len1 < 1)
	{
		free(str_cpy);
		return (ft_strdup(str));
	}
	result = ft_strnstr(str_cpy, expel, len1);
	if (!result)
	{
		free(str_cpy);
		return (NULL);
	}
	cut_connect(&str_cpy, result - str_cpy, len2);
	return (str_cpy);
}

char	*ft_strexpel(char *str, const char *expel, t_err mode)
{
	char	*str_cpy;
	char	*tmp;

	if (!str || !expel)
		return (NULL);
	str_cpy = ft_strdup(str);
	if (!str_cpy)
		return (NULL);
	while (ft_strnstr(str_cpy, expel, ft_strlen(str_cpy)))
	{
		tmp = expel_single(str_cpy, expel);
		free(str_cpy);
		if (!tmp)
			return (NULL);
		str_cpy = tmp;
		if (mode == ONE)
			break ;
	}
	return (str_cpy);
}
