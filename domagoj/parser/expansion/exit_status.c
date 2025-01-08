/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 18:31:12 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/07 20:08:54 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static char	*fetch_ex_cmd(void)
{
	int		ex_code;
	char	*return_str;
	t_data	*data;

	data = get_data(NULL);
	ex_code = data->last_ex_code;
	return_str = ft_itoa(ex_code);
	return (return_str);
}

int	expand_exit_status(char **str)
{
	int		*indexes;
	int		i;
	char	*status;
	size_t	offset;

	i = 0;
	offset = 0;
	if (!*str || !str)
		return (-1);
	status = fetch_ex_cmd();
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
