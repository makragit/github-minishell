/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:47:06 by mkrausho          #+#    #+#             */
/*   Updated: 2025/02/14 22:20:38 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// returns 0 on SUCCESS, errno or 1 on Failure
int	builtin_unset(char **args)
{
	int	i;
	int	search;

	if (mak_arr_size(args) == 1)
		return (0);
	if (args[1][0] == '-' && args[1][1] != '\0')
		return (bsh_err("unset", args[1], "invalid option", 2));
	i = 1;
	while (args[i])
	{
		search = search_key_in_array(get_data(NULL)->mini_envp, args[i]);
		if (env_key_valid(args[i]) >= 2 || search < 1)
		{
			i++;
			continue ;
		}
		if (!array_free_and_rem(&get_data(NULL)->mini_envp, args[i]))
			return (1);
		i++;
	}
	return (0);
}
