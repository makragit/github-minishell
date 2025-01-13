/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_append.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 18:10:57 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/07 19:44:19 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_append(const char *str, int fd)
{
	ssize_t		written;
	size_t		len;
	size_t		total_written;

	total_written = 0;
	if (!str || fd < 0)
		return (-1);
	len = 0;
	while (str[len])
		len++;
	while (total_written < len)
	{
		written = write(fd, str + total_written, len - total_written);
		if (written == -1)
			return (-1);
		total_written += written;
	}
	return (0);
}
