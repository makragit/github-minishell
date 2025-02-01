/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:58:01 by dbogovic          #+#    #+#             */
/*   Updated: 2025/02/01 19:03:50 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	syntax_error_print(char *reason)
{
	const char	*prefix = "Minishell: syntax error near unexpected token `";
	const char	*suffix = "'\n";
	t_data		*data;

	data = get_data(NULL);
	write(STDERR_FILENO, prefix, ft_strlen(prefix));
	if (reason)
		write(STDERR_FILENO, reason, ft_strlen(reason));
	write(STDERR_FILENO, suffix, ft_strlen(suffix));
	data->last_ex_code = 2;
	return (1);
}

void	print_path_err(t_err reason, const char *cmd)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	if (reason == PERMISSION)
	{
		get_data(NULL)->last_ex_code = 126;
		write(2, "Permission denied\n", 18);
	}
	else if (reason == DIR)
	{
		get_data(NULL)->last_ex_code = 126;
		write(2, "Is a directory\n", 15);
	}
	else if (reason == NOT_FOUND)
	{
		get_data(NULL)->last_ex_code = 127;
		write(2, "command not found\n", 18);
	}
	else if (reason == MISSING)
	{
		get_data(NULL)->last_ex_code = 127;
		write(2, "No such file or directory\n", 26);
	}
	else if (reason == PERRORR)
		perror("");
}
