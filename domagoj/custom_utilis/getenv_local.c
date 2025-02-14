/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getenv_local.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 18:36:51 by dbogovic          #+#    #+#             */
/*   Updated: 2025/02/14 20:09:52 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*getenv_local(char *name)
{
	char		**env;
	size_t		line;
	t_data		*shared_data;

	line = 0;
	shared_data = get_data(NULL);
	if (!shared_data)
		return (NULL);
	if (!shared_data->mini_envp)
		return (NULL);
	env = shared_data->mini_envp;
	while (env[line])
	{
		if (ft_strncmp(name, env[line], ft_strlen(name)) == OK)
		{
			if (env[line][ft_strlen(name)] == '=')
				return (env[line] + (ft_strlen(name) + 1));
		}
		line++;
	}
	return (NULL);
}
