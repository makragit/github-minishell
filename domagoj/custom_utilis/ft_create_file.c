/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_file.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 18:09:54 by dbogovic          #+#    #+#             */
/*   Updated: 2025/02/11 15:14:25 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*generate_filename(void)
{
	char		*str_pid;
	static int	counter = 0;

	str_pid = ft_itoa(counter);
	if (!str_pid)
		return (NULL);
	counter++;
	return (str_pid);
}

int	ft_create_file(char **filename)
{
	int			fd;

	*filename = generate_filename();
	if (!*filename)
		return (-1);
	fd = open((*filename), O_CREAT | O_EXCL | O_RDWR | O_APPEND, 0600);
	if (fd == -1)
	{
		free(*filename);
		*filename = NULL;
		return (-1);
	}
	return (fd);
}
