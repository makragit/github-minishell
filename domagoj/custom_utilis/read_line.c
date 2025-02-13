/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 13:44:43 by dbogovic          #+#    #+#             */
/*   Updated: 2025/02/11 15:01:38 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*ensure_capacity(char *buf, size_t *buf_size, size_t needed)
{
	size_t	new_size;
	char	*new_buf;

	if (needed <= *buf_size)
		return (buf);
	if (*buf_size == 0)
		new_size = 64;
	else
		new_size = (*buf_size * 2);
	while (new_size < needed)
		new_size *= 2;
	new_buf = malloc(new_size);
	ft_memset(new_buf, 0, new_size);
	if (!new_buf)
	{
		free(buf);
		return (NULL);
	}
	if (buf && *buf_size > 0)
		ft_memcpy(new_buf, buf, *buf_size);
	free(buf);
	*buf_size = new_size;
	return (new_buf);
}

char	*ret_value(ssize_t ret, size_t l, size_t s, char *buf)
{
	if (l == 0 || ret < 0)
	{
		free(buf);
		return (NULL);
	}
	return (ensure_capacity(buf, &s, l + 1));
}

char	*read_line(void)
{
	char	*buf;
	size_t	buf_size;
	size_t	buf_len;
	char	c;
	ssize_t	ret;

	buf = NULL;
	buf_size = 0;
	buf_len = 0;
	write(STDOUT_FILENO, ">", 1);
	while (1)
	{
		ret = read(STDIN_FILENO, &c, 1);
		if (ret <= 0)
			return (ret_value(ret, buf_len, buf_size, buf));
		if (c == '\n')
			return (ensure_capacity(buf, &buf_size, buf_len + 1));
		buf = ensure_capacity(buf, &buf_size, buf_len + 1);
		if (!buf)
			return (NULL);
		buf[buf_len] = c;
		buf_len++;
	}
}
