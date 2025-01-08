/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_file.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 18:09:54 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/07 20:19:06 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_create_file(void)
{
	char		*filename;
	int			fd;
	char		*tmp;
	char		*counter_str;
	static int	counter = 0;

	tmp = ft_itoa(getpid());
	if (!tmp)
		return (-1);
	counter++;
	counter_str = ft_itoa(counter);
	if (!counter_str)
	{
		free(tmp);
		return (-1);
	}
	filename = ft_strjoin(tmp, counter_str);
	free(tmp);
	free(counter_str);
	if (!filename)
		return (-1);
	fd = open(filename, O_CREAT | O_EXCL | O_RDWR | O_APPEND, 0600);
	if (fd == -1)
	{
		perror("open");
		free(filename);
		return (-1);
	}
	if (unlink(filename) == -1) //it will del file if when fd is closed
	{
		perror("unlink");
		free(filename);
		close(fd);
		return (-1);
	}
	free(filename);
	return (fd);
}
