/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:27:10 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/03 18:34:11 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

#include <stddef.h>
#include <stdbool.h>

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

static int check_redirs(const char *input)
{
	size_t	i;

	i = 0;
	while (input[i])
	{
		if (i == 0)
		{
			if (input[i] == '>' || input[i] == '#')
			{
				printf("Input error!\n");
				return (1);
			}
		}
		if (!input[i + 1])
		{
			if (input[i] == '<' || input[i] == '>')
			{
				printf("Input error!\n");
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	input_check(const char *input)
{
	size_t	i;

	i = 0;
	if (check_redirs(input) == 1)
	{
		printf("Input error\n");
		return (1);
	}
	if (check_quotes(input) == 1)
	{
		printf("Input error\n");
		return (1);
	}
	while (input[i])
	{
		if (input[i] == '|')
		{
			if (!i || !input[i + 1])
			{
				printf("Input error!\n");
				return (1);
			}
			else if (i)
			{
				if (input[i - 1] == '|')
				{
					printf("Input error!\n");
					return (1);
				}
			}
		}
		i++;
	}
	return (0);
}
