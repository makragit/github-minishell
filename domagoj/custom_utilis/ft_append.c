/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_append.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 18:10:57 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/05 18:33:18 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int ft_append(const char *str, int fd)
{
	ssize_t written;
	size_t len;
	size_t total_written = 0;

	if (!str || fd < 0)
		return -1; // Handle invalid input

	len = 0;
	while (str[len]) // Calculate the length of the string
		len++;

	while (total_written < len)
	{
		written = write(fd, str + total_written, len - total_written);
		if (written == -1) // Check for errors in write()
			return -1;
		total_written += written; // Track the total number of bytes written
	}

	return 0; // Return 0 to indicate success
}

