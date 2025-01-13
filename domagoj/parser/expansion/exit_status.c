/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 18:31:12 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/10 16:42:24 by dbogovic         ###   ########.fr       */
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

char	*process_expanding(char *str, int *indexes, char *status)
{
	int			i;
	size_t		offset;
	char		*s_cpy;
	char		*cpy;

	i = 0;
	offset = 0;
	s_cpy = ft_strdup(str);
	if (!s_cpy)
		return (NULL);
	while (indexes[i] != -1)
	{
		if (ft_str_insert(&s_cpy, status, indexes[i++] + offset) == -1)
		{
			free(s_cpy);
			return (NULL);
		}
		offset += ft_strlen(status);
	}
	cpy = ft_strexpel(s_cpy, "$?", ALL);
	free(s_cpy);
	return (cpy);
}

int	exit_status(char **str)
{
	int		*indexes;
	char	*insert;
	char	*expanded_str;

	insert = fetch_ex_cmd();
	if (!insert)
		return (-1);
	indexes = find_indexes(*str, "$?");
	if (!indexes)
	{
		free(insert);
		return (0);
	}
	expanded_str = process_expanding(*str, indexes, insert);
	free(indexes);
	free(insert);
	if (!expanded_str)
		return (-1);
	free(*str);
	*str = expanded_str;
	return (0);
}
