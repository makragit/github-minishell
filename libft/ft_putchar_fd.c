/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:10:59 by mkrausho          #+#    #+#             */
/*   Updated: 2024/04/16 20:40:35 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Name:           ft_putchar_fd
Protoype:       void ft_putchar_fd(char c, int fd);
Parameters:     c: The character to output.
                fd: The file descriptor on which to write.
Return:         None
External:       write
Description:    Outputs the character ’c’ to the given file
                descriptor.
*/

#include <unistd.h>

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
