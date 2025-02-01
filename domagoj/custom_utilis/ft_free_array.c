/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:35:33 by dbogovic          #+#    #+#             */
/*   Updated: 2025/02/01 13:58:56 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_free_array(char **arr)
{
	size_t	c;

	c = 0;
	if (!arr)
		return ;
	while (arr[c])
	{
		free(arr[c]);
		arr[c] = NULL;
		c++;
	}
	free(arr);
	arr = NULL;
}
