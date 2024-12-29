/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:26:52 by mkrausho          #+#    #+#             */
/*   Updated: 2024/04/16 20:46:31 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Name			ft_putendl_fd
Prototype		void ft_putendl_fd(char *s, int fd);
Parameters		s: The string to output.
				fd: The file descriptor on which to write.
Return			None
external		write
Description		Outputs the string ’s’ to the given file descriptor
				followed by a newline.
*/

#include <unistd.h>

void	ft_putendl_fd(char *s, int fd)
{
	int		i;
	char	end;

	if (!s)
		return ;
	i = 0;
	while (s[i])
		i++;
	write(fd, s, i);
	end = '\n';
	write(fd, &end, 1);
}
