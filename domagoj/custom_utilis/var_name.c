/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_name.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 09:50:13 by dbogovic          #+#    #+#             */
/*   Updated: 2025/02/10 09:50:36 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*_var_name(char **str, size_t start, size_t len)
{
	char	*tmp;
	char	*part;

	part = ft_substr(*str, start, len);
	if (!part)
		return (NULL);
	tmp = ft_strexpel(*str, part, ONE);
	if (!tmp)
	{
		free(part);
		return (NULL);
	}
	free(*str);
	*str = tmp;
	return (part);
}
