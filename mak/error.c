/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:31:46 by mkrausho          #+#    #+#             */
/*   Updated: 2025/02/01 19:31:47 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exit_error(char *s)
{
	perror(s);
	free_all();
	if (errno == 0)
		errno = 1;
	exit(errno);
}

void	malloc_error(char *s)
{
	perror(s);
	free_all();
	if (errno == 0)
		errno = 1;
	exit(errno);
}

int	bsh_err(char *cmd, char *arg, char *msg, int code)
{
	char	*ret_msg;
	int		size;
	int		quotes;

	if (!cmd || !msg)
	{
		ft_putendl_fd("minishell: Error", 2);
		return (code);
	}
	quotes = (arg && (ft_strncmp(cmd, "export", 6) == 0
				|| ft_strncmp(cmd, "unset", 5) == 0
				|| ft_strncmp(cmd, "env", 3) == 0));
	size = ft_strlen("minishell: ") + ft_strlen(cmd) + ft_strlen(msg) + 2;
	if (arg)
		size += ft_strlen(arg) + 2 + quotes + quotes;
	ret_msg = bsh_err_help(cmd, arg, size, quotes);
	ft_strlcat(ret_msg, msg, size + 1);
	ft_putendl_fd(ret_msg, 2);
	free(ret_msg);
	return (code);
}

char	*bsh_err_help(char *cmd, char *arg, int size, int qte)
{
	char	*ret_msg;

	ret_msg = (char *)ft_calloc(sizeof(char), size + 1);
	if (!ret_msg)
		malloc_error("ERROR: malloc failed in bsh_err");
	ret_msg[size] = '\0';
	ft_strlcat(ret_msg, "minishell: ", size + 1);
	ft_strlcat(ret_msg, cmd, size + 1);
	ft_strlcat(ret_msg, ": ", size + 1);
	if (arg)
	{
		if (qte)
			ft_strlcat(ret_msg, "`", size + 1);
		ft_strlcat(ret_msg, arg, size + 1);
		if (qte)
			ft_strlcat(ret_msg, "'", size + 1);
		ft_strlcat(ret_msg, ": ", size + 1);
	}
	return (ret_msg);
}
