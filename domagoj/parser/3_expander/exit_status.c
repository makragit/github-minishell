/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 18:31:12 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/03 18:33:14 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	expand_exit_status(char **str)
{
	int		*indexes;
	int		i;
	int		last_executed_cmd;
	char	*status;
	size_t	offset;

	last_executed_cmd = 0;
	i = 0;
	offset = 0;
	if (!*str || !str)
		return (-1);
	status = ft_itoa(last_executed_cmd);
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
	status = ft_strexpel(*str, "$?");
	free(*str);
	*str = status;
	free(indexes);
	return (0);
}
