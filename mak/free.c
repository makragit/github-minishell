/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:31:51 by mkrausho          #+#    #+#             */
/*   Updated: 2025/02/01 19:31:52 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_data(void)
{
	t_data	*data;

	rl_clear_history();
	data = get_data(NULL);
	if (!data)
		return ;
	if (data->prompt_str)
		free(data->prompt_str);
	if (data->last_cwd)
		free(data->last_cwd);
	if (data->input)
		free(data->input);
	if (data->env_paths)
		mak_free_array(data->env_paths);
	if (data->mini_envp)
		mak_free_array(data->mini_envp);
	free(data);
}

void	free_all(void)
{
	t_cmd_table	*table;

	free_data();
	table = get_table_reset(NULL, 0);
	if (!table)
		return ;
	else
		free_table(table);
}

int	mak_free_array(char **arr)
{
	size_t	i;

	if (!arr)
		return (0);
	i = 0;
	while (arr[i])
	{
		free (arr[i]);
		i++;
	}
	free(arr);
	return (0);
}
