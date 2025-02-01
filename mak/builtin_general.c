/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_general.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:33:03 by mkrausho          #+#    #+#             */
/*   Updated: 2025/02/01 19:33:04 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	try_builtin(t_cmd_table *table)
{
	if (!table)
		return (-1);
	if (ft_strncmp(table->cmd, "cd", 2) == 0 && table->cmd[2] == '\0')
		return (builtin_chdir(table->args));
	else if (ft_strncmp(table->cmd, "echo", 4) == 0 && table->cmd[4] == '\0')
		return (builtin_echo(table->args));
	else if (ft_strncmp(table->cmd, "pwd", 3) == 0 && table->cmd[3] == '\0')
		return (builtin_pwd());
	else if (ft_strncmp(table->cmd, "env", 3) == 0 && table->cmd[3] == '\0')
		return (builtin_env(table->args));
	else if (ft_strncmp(table->cmd, "export", 6) == 0 && table->cmd[6] == '\0')
		return (builtin_export(table->args));
	else if (ft_strncmp(table->cmd, "unset", 5) == 0 && table->cmd[5] == '\0')
		return (builtin_unset(table->args));
	else if (ft_strncmp(table->cmd, "exit", 4) == 0 && table->cmd[4] == '\0')
		return (builtin_exit(table->args));
	return (-1);
}

int	builtin_pwd(void)
{
	char	*cwd_path;

	cwd_path = get_cwd_path();
	if (cwd_path == NULL)
	{
		if (get_data(NULL)->last_cwd == NULL)
			printf("(null)\n");
		else
			printf("%s\n", get_data(NULL)->last_cwd);
		return (0);
	}
	printf("%s\n", cwd_path);
	free(cwd_path);
	return (0);
}

// returns 0 on SUCCESS, errno or 1 on Failure
int	builtin_env(char **args)
{
	char	**env;

	if (mak_arr_size(args) > 1)
		return (bsh_err("env", args[1], "No such file or directory", 0));
	env = get_data(NULL)->mini_envp;
	while (*env != NULL)
	{
		printf("%s\n", *env);
		env++;
	}
	return (0);
}

// return 0 on Success, errno or 1 on Failure
int	builtin_echo(char **args)
{
	int	i;
	int	option_found;

	if (!args || !*args)
		return (1);
	if (mak_arr_size(args) == 1)
	{
		printf("\n");
		return (0);
	}
	option_found = 0;
	if (check_cmd_option(args[1], 'n'))
		option_found = 1;
	i = 1 + option_found;
	if (args[i])
		printf("%s", args[i++]);
	while (args[i])
	{
		printf(" %s", args[i]);
		i++;
	}
	if (!option_found)
		printf("\n");
	return (0);
}

int	builtin_exit(char **args)
{
	int	ret;

	get_data(NULL)->exit_called = 1;
	if (mak_arr_size(args) == 1)
	{
		printf("exit\n");
		return (errno);
	}
	if (mak_arr_size(args) > 2)
	{
		get_data(NULL)->exit_called = 0;
		printf("exit\n");
		return (bsh_err("exit", args[1], "too many arguments", 1));
	}
	if (!is_numeric(args[1]))
	{
		printf("exit\n");
		return (bsh_err("exit", args[1], "numeric argument required", 2));
	}
	ret = ft_atoi(args[1]);
	ret = (unsigned char)ret;
	printf("exit\n");
	return (ret);
}
