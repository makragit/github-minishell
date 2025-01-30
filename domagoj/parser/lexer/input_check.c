/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:27:10 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/25 19:19:51 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static int	check_quotes(const char *input)
{
	size_t	i;
	char	open_quote;

	i = 0;
	open_quote = '\0';
	while (input[i])
	{
		if ((input[i] == '\'' || input[i] == '\"'))
		{
			if (open_quote == '\0')
				open_quote = input[i];
			else if (open_quote == input[i])
				open_quote = '\0';
		}
		i++;
	}
	return (open_quote != '\0');
}

int	input_check(const char *input)
{
	if (check_quotes(input) == 1)
	{
		syntax_error_print("quote");
		return (1);
	}
	return (0);
}
