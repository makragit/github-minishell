/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_file.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 18:09:54 by dbogovic          #+#    #+#             */
/*   Updated: 2025/02/15 12:53:09 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	fill_str(char **str, int fd)
{
	size_t			count;
	unsigned char	c;

	count = 0;
	while (count < 8)
	{
		if (read(fd, &c, 1) != 1)
		{
			free(*str);
			*str = NULL;
			break ;
		}
		if (ft_isalnum(c))
			(*str)[count++] = c;
	}
}

static char	*generate_filename(void)
{
	char			*filename;
	int				fd;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (NULL);
	filename = malloc(9 * sizeof(char));
	if (!filename)
	{
		close(fd);
		return (NULL);
	}
	ft_memset(filename, 0, ft_strlen(filename));
	fill_str(&filename, fd);
	close(fd);
	return (filename);
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
