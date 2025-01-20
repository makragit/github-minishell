/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 14:16:40 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/19 14:33:40 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_builtin(char *cmd)
{
	size_t	len;

	len = ft_strlen(cmd);
	if (ft_strncmp(cmd, "cd", 2) == 0 && len == 2)
		return (1);
	else if (ft_strncmp(cmd, "echo", 4) == 0 && len == 4)
		return (1);
	else if (ft_strncmp(cmd, "pwd", 3) == 0 && len == 3)
		return (1);
	else if (ft_strncmp(cmd, "env", 3) == 0 && len == 3)
		return (1);
	else if (ft_strncmp(cmd, "export", 6) == 0 && len == 6)
		return (1);
	else if (ft_strncmp(cmd, "unset", 5) == 0 && len == 5)
		return (1);
	else if (ft_strncmp(cmd, "exit", 4) == 0 && len == 4)
		return (1);
	return(0);
}
