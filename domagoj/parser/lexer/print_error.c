/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:58:01 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/10 16:42:37 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	syntax_error_print(char *reason)
{
	const char	*prefix = "Minishell: syntax error near unexpected token `";
	const char	*suffix = "'\n";
	t_data		*data;

	data = get_data(NULL);
	write(STDERR_FILENO, prefix, strlen(prefix));
	if (reason)
		write(STDERR_FILENO, reason, strlen(reason));
	write(STDERR_FILENO, suffix, strlen(suffix));
	data->last_ex_code = 2;
}
