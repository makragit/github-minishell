/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 18:31:12 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/04 22:52:59 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	expand_exit_status(char **str)
{
	int		*indexes;
	int		i;
	int		g_last_executed_cmd;
	char	*status;
	size_t	offset;

	g_last_executed_cmd = 0;
	i = 0;
	offset = 0;
	if (!*str || !str)
		return (-1);
	status = ft_itoa(g_last_executed_cmd);
	if (!status)
		return (-1);
	indexes = find_indexes(*str, "$?");
	if (!indexes)
	{
		free(status);
		return (0);
	}
	while (indexes[i] != -1)
	{
		if (ft_str_insert(str, status, indexes[i++] + offset) == -1)
		{
			free(status);
			free(indexes);
			return (-1);
		}
		offset += ft_strlen(status);
	}
	free(status);
	free(indexes);
	status = ft_strexpel(*str, "$?");
	if (!status)
		return (-1);
	free(*str);
	*str = status;
	return (0);
}
