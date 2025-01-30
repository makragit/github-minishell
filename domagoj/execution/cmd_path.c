/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:22:58 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/29 17:33:29 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

t_err	is_proper_exe(const char *path, int flag)
{
	struct stat	st;

	if (stat(path, &st) == -1)
	{
		if (flag == 1)
			return (NOT_FOUND);
		else
			return (MISSING);
	}
	if (S_ISDIR(st.st_mode))
	{
		return (DIR);
	}
	if (access(path, X_OK) != 0)
	{
		return (PERMISSION);
	}
	return (OK);
}

static char	*get_valid_path(char **arr, t_err *last_err)
{
	struct stat	st;
	size_t		c;
	char		*path;

	c = 0;
	*last_err = OK;
	path = NULL;
	while (arr[c])
	{
		*last_err = is_proper_exe(arr[c], 1);
		if (*last_err == OK)
			break ;
		c++;
	}
	if (arr[c])
		path = ft_strdup(arr[c]);
	ft_free_array(arr);
	arr = NULL;
	return (path);
}

char	*get_path(const char *cmd)
{
	char	*cmd_tmp;
	char	**arr;
	t_err	reason;

	if (is_cmd_path(cmd))
		return (ft_strdup(cmd));
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
	cmd_tmp = get_valid_path(arr, &reason);
	if (reason != OK)
		print_path_err(reason, cmd);
	return (cmd_tmp);
}
