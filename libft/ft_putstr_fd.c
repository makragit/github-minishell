/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:12:47 by mkrausho          #+#    #+#             */
/*   Updated: 2024/04/16 20:43:36 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Name			ft_putstr_fd
Prototype		void ft_putstr_fd(char *s, int fd);
Parameters		s: The string to output.
				fd: The file descriptor on which to write.
Return			None
External		write
Description		Outputs the string ’s’ to the given file
				descriptor.
*/

#include <unistd.h>

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
		i++;
	write(fd, s, i);
}
