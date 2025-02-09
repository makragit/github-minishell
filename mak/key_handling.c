/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:32:34 by mkrausho          #+#    #+#             */
/*   Updated: 2025/02/01 20:06:55 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// returns how many times 'search' was found using in array
// New: now only compares until '='
int	search_key_in_array(char **arr, char *search)
{
	int	i;
	int	found;

	found = 0;
	i = 0;
	while (arr[i])
	{
		if (ft_strncmp(arr[i], search, len_to_equal_sign(search)) == 0)
			found++;
		i++;
	}
	return (found);
}

// returns 0 if invalid
// returns 1 if valid and without '=' at the end
// returns 2 if valid and WITH '=' at the end
// returns 3 not ending with '=' (abc=test)
int	env_key_valid(char *str)
{
	int	i;

	if (!str || str[0] == '=')
		return (0);
	if (!((str[0] >= 'A' && str[0] <= 'Z')
			|| (str[0] >= 'a' && str[0] <= 'z')
			|| (str[0] == '_')))
		return (0);
	i = 1;
	while (str[i] != '\0' && str[i] != '=')
	{
		if (!env_key_valid_helper(str[i]))
			return (0);
		i++;
	}
	if (str[i] == '=')
	{
		if (str[i + 1] != '\0')
			return (3);
		else
			return (2);
	}
	return (1);
}

int	env_key_valid_helper(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9')
		|| (c == '_'))
		return (1);
	else
		return (0);
}

// updates or creates ENV if not found
// 		NEEDS "PWD=" with '=' as key (for now)
void	update_env_var(char *key, char *value)
{
	int		search;
	char	*key_value;

	if (!key || !value)
		return ;
	key_value = ft_strjoin(key, value);
	if (!key_value)
		malloc_error("update_env_var: malloc error");
	if (env_key_valid(key) != 2)
		return ;
	search = search_key_in_array(get_data(NULL)->mini_envp, key);
	if (search <= 0)
	{
		if (array_free_and_add(&get_data(NULL)->mini_envp, key_value) == -1)
		{
			free(key_value);
			malloc_error("update_env_var: malloc error");
			return ; // fix?
		}
	}
	if (search == 1)
		update_env_var_help(key, key_value);
	free(key_value);
	return ;
}

void	update_env_var_help(char *key, char *key_value)
{
	int		i;
	char	**temp_envp;

	temp_envp = get_data(NULL)->mini_envp;
	i = 0;
	while (temp_envp[i])
	{
		if (ft_strncmp(temp_envp[i], key, ft_strlen(key)) == 0)
		{
			free(temp_envp[i]);
			temp_envp[i] = ft_strdup(key_value);
			break ;
		}
		i++;
	}
	return ;
}
