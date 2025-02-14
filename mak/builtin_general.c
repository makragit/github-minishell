/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_general.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:33:03 by mkrausho          #+#    #+#             */
/*   Updated: 2025/02/11 16:10:09 by dbogovic         ###   ########.fr       */
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
		return (builtin_pwd(table->args));
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

int	builtin_pwd(char **args)
{
	char	*cwd_path;

	if (mak_arr_size(args) > 1)
		if (args[1][0] == '-' && args[1][1] != '\0')
			return (bsh_err("pwd", args[1], "invalid option", 2));
	cwd_path = get_cwd_path();
	if (cwd_path == NULL)
	{
		if (get_data(NULL)->last_cwd)
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
	while (check_cmd_option(args[option_found + 1], 'n') == 1)
		option_found++;
	i = 1 + option_found;
	if (args[i])
		printf("%s", check_tilde(args[i++]));
	while (args[i])
	{
		printf(" %s", check_tilde(args[i]));
		i++;
	}
	if (!option_found)
		printf("\n");
	return (0);
}

int	builtin_exit(char **args)
{
	long	ret;

	get_data(NULL)->exit_called = 1;
	if (mak_arr_size(args) == 1)
	{
		write(2, "exit\n", 5);
		return (get_data(NULL)->last_ex_code);
	}
	if (!is_numeric(args[1]))
	{
		return (bsh_err("exit", args[1], "numeric argument required", 2));
	}
	if (mak_arr_size(args) > 2)
	{
		get_data(NULL)->exit_called = 0;
		return (bsh_err("exit", args[1], "too many arguments", 1));
	}
	ret = mak_atol(args[1]);
	ret = (unsigned char)ret;
	return (ret);
}

long	mak_atol(const char *str)
{
	long result;
	int sign;
	int i;
	
	result = 0;
	sign = 1;
	i = 0;
	while (isspace(str[i]))
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	} 
	else if (str[i] == '+')
		i++;
	
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}
