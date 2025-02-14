/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:27:10 by domagoj           #+#    #+#             */
/*   Updated: 2025/02/14 19:39:58 by dbogovic         ###   ########.fr       */
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

char	*input_check(char *input)
{
	if (!input)
		return (NULL);
	if (check_quotes(input) == 1)
	{
		syntax_error_print("quote");
		free(input);
		return (NULL);
	}
	return (input);
}
