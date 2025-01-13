/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:22:58 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/10 16:41:54 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_cmd_path(const char *cmd)
{
	size_t	c;

	c = 0;
	while (cmd[c])
	{
		if (cmd[c] == '/')
			return (1);
		c++;
	}
	return (0);
}

static char	**refill_arr(char **org, char *cmd)
{
	size_t	c;
	char	*tmp;

	c = 0;
	while (org[c])
	{
		tmp = ft_strjoin(org[c], cmd);
		if (!tmp)
		{
			free(tmp);
			ft_free_array(org);
			return (NULL);
		}
		free(org[c]);
		org[c] = tmp;
		c++;
	}
	return (org);
}

char	**get_path(const char *cmd)
{
	char	*cmd_tmp;
	char	**arr;

	if (is_cmd_path(cmd))
		return (ft_split(cmd, ' '));
	cmd_tmp = ft_strjoin("/", cmd);
	if (!cmd_tmp)
		return (NULL);
	arr = ft_split(getenv_local("PATH"), ':');
	if (!arr)
	{
		free(cmd_tmp);
		return (NULL);
	}
	arr = refill_arr(arr, cmd_tmp);
	free(cmd_tmp);
	return (arr);
}
