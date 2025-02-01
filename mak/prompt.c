/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:32:14 by mkrausho          #+#    #+#             */
/*   Updated: 2025/02/01 19:32:15 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*display_prompt(void)
{
	char	cwd[PATH_MAX];
	char	*user;
	int		prompt_len;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		if (get_data(NULL)->prompt_str == NULL)
			return (readline("(no current working directory): "));
		else
			return (readline(get_data(NULL)->prompt_str));
	}
	free(get_data(NULL)->last_cwd);
	get_data(NULL)->last_cwd = get_cwd_path();
	user = getenv_local("USER");
	if (user == NULL)
		user = "(null)";
	prompt_len = ft_strlen("\033[1;36m") + ft_strlen(user)
		+ ft_strlen(cwd) + ft_strlen("\033[0m") + 4;
	free(get_data(NULL)->prompt_str);
	get_data(NULL)->prompt_str = prepare_prompt_string(user, cwd, prompt_len);
	return (readline(get_data(NULL)->prompt_str));
}

char	*prepare_prompt_string(char *user, char *prompt_path, int size)
{
	char	*str;

	str = (char *)ft_calloc(sizeof(char), size + 1);
	if (!str)
		malloc_error("ERROR: malloc failed in prepare_prompt_string");
	str[size] = '\0';
	ft_strlcat(str, "\033[1;36m", size + 1);
	ft_strlcat(str, user, size + 1);
	ft_strlcat(str, ":", size + 1);
	ft_strlcat(str, prompt_path, size + 1);
	if (is_root())
		ft_strlcat(str, "# ", size + 1);
	else
		ft_strlcat(str, "$ ", size + 1);
	ft_strlcat(str, "\033[0m", size + 1);
	return (str);
}

int	is_root(void)
{
	char	*user;

	user = getenv_local("USER");
	if (user == NULL)
		return (0);
	if (ft_strncmp(user, "root", 4) == 0)
		return (1);
	return (0);
}
