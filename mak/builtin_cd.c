/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:32:48 by mkrausho          #+#    #+#             */
/*   Updated: 2025/02/14 17:52:34 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// returns 0 for success, errno or 1 on failure
int	builtin_chdir(char **args)
{
	char	*path;

	if (args == NULL)
		return (1);
	if (mak_arr_size(args) > 2)
		return (bsh_err("cd", NULL, "too many arguments", 1));
	if ((mak_arr_size(args) == 1)
		|| (ft_strlen(args[1]) == 1 && args[1][0] == '~'))
	{
		path = getenv_local("HOME");
		if (path == NULL)
			return (bsh_err("cd", NULL, "HOME not set", 1));
	}
	else if (ft_strlen(args[1]) == 1 && args[1][0] == '-')
	{
		path = getenv_local("OLDPWD");
		if (!path)
			return (bsh_err("cd", NULL, "OLDPWD not set", 1));
	}
	else
		path = args[1];
	if (chdir(path) != 0)
		return (bsh_err("cd", NULL, "No such file or directory", 1));
	builtin_chdir_update_pwd();
	return (cwd_deleted());
}

void	builtin_chdir_update_pwd(void)
{
	char	*cwd_path;
	char	*last_cwd_cpy;

	cwd_path = get_cwd_path();
	if (cwd_path == NULL)
		return ;
	update_env_var("PWD=", cwd_path);
	update_env_var("OLDPWD=", get_data(NULL)->last_cwd);
	free(get_data(NULL)->last_cwd);
	last_cwd_cpy = ft_strdup(cwd_path);
	if (last_cwd_cpy == NULL)
		malloc_error("builtin_chdir_update_pwd malloc error");
	get_data(NULL)->last_cwd = last_cwd_cpy;
	free(cwd_path);
	return ;
}

char	*check_tilde(char *str)
{
	if (ft_strncmp(str, "~", 1) == 0 && ft_strlen(str) == 1)
		return (getenv_local("HOME"));
	return (str);
}
