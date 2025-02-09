/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:32:55 by mkrausho          #+#    #+#             */
/*   Updated: 2025/02/01 19:32:56 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_numeric(const char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

// returns 1 for -n -nnnn.., 0 for -nnnnx, ----n etc
// returns -1 if '-' found but option invalid 
// 		(for '-c' check in non-interactive shell)
int	check_cmd_option(char *str, char option)
{
	int	i;

	if (!str)
		return (0);
	if (ft_strlen(str) < 2)
		return (0);
	if (str[0] != '-')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != option)
			return (-1);
		i++;
	}
	return (1);
}

// returns malloced 'current working directory', 
// 		exits program on getcwd() failure
char	*get_cwd_path(void)
{
	char	cwd[PATH_MAX];
	char	*ret;
	int		i;
	char	*ret_null;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		ret_null = ft_strdup("(null)");
		if (!ret_null)
			malloc_error("ERROR: malloc failed in get_cwd_path: ft_strdup");
		return (ret_null);
	}
	ret = (char *)malloc(sizeof(char) * (ft_strlen(cwd) + 1));
	if (ret == NULL)
		malloc_error("ERROR: malloc failed in get_cwd_path");
	i = 0;
	while (cwd[i])
	{
		ret[i] = cwd[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

int	len_to_equal_sign(char *str)
{
	int	len;

	len = 0;
	while (str[len] && str[len] != '=')
		len++;
	return (len);
}

int	ft_isspace(char c)
{
	if (c == ' ')
		return (1);
	return (0);
}
