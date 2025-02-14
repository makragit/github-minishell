/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:31:33 by mkrausho          #+#    #+#             */
/*   Updated: 2025/02/14 17:41:36 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_data	*get_data(t_data *data)
{
	static t_data	*resource = NULL;

	if (data)
		resource = data;
	return (resource);
}

t_cmd_table	*get_table_reset(t_cmd_table *table, int reset)
{
	static t_cmd_table	*resource = NULL;

	if (reset)
		resource = NULL;
	else if (table != NULL)
		resource = table;
	return (resource);
}

t_data	*init_data(char **envp)
{
	t_data	*data;

	data = (t_data *)malloc(sizeof(t_data) * 1);
	if (!data)
	{
		malloc_error("ERROR: malloc failed in init_data");
		return (NULL);
	}
	ft_memset(data, 0, sizeof(t_data));
	get_data(data);
	data->mini_envp = copy_array(envp);
	if (data->mini_envp == NULL)
		data->mini_envp = create_empty_envp();
	data->last_cwd = get_cwd_path();
	return (data);
}

char	**create_empty_envp(void)
{
	char	**empty_envp;
	char	*cwd_path;

	empty_envp = malloc(sizeof(char *) * 2);
	if (!empty_envp)
	{
		malloc_error("ERROR: malloc failed in init_data");
		return (NULL);
	}
	cwd_path = get_cwd_path();
	if (cwd_path == NULL)
		empty_envp[0] = ft_strdup("PWD=");
	else
		empty_envp[0] = ft_strjoin("PWD=", cwd_path);
	if (!empty_envp[0])
	{
		free(cwd_path);
		free(empty_envp);
		malloc_error("ERROR: malloc failed in init_data");
		return (NULL);
	}
	empty_envp[1] = NULL;
	free(cwd_path);
	return (empty_envp);
}
