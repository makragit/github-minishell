/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_insert_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 20:26:17 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/04 22:52:12 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_str_insert(char **src, char *insert, size_t place)
{
	char		*joined;
	size_t		len1;
	size_t		len2;

	if (!*src || !src || !insert)
		return (-1);
	len1 = ft_strlen(*src);
	len2 = ft_strlen(insert);
	if (len2 == 0)
		return (0);
	if (place > len1)
		return (-1);
	joined = malloc(sizeof(char) * (len1 + len2 + 1));
	if (!joined)
		return (-1);
	ft_strlcpy(joined, (*src), place + 1);
	ft_strlcat(joined, insert, len1 + len2 + 1);
	ft_strlcat(joined, (*src) + place, len1 + len2 + 1);
	free(*src);
	*src = joined;
	return (0);
}
