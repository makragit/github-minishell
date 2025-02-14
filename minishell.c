/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:31:22 by mkrausho          #+#    #+#             */
/*   Updated: 2025/02/11 16:10:50 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;
	int		ret;
	int		non_interactive_mode;

	if (!envp)
		return (0);
	non_interactive_mode = check_argv(argc, argv);
	data = init_data(envp);
	get_data(data);
	set_signals(non_interactive_mode);
	if (non_interactive_mode == 1)
		ret = run_non_interactive(argv);
	else
		ret = run_interactive_test(data);
		/* ret = run_interactive(data); */
	free_all();
	return (ret);
}

// runs minishell in interactive, returns last exit code
int	run_interactive(t_data *data)
{
	t_cmd_table	*table;

	while (data->exit_called == 0)
	{
		free(data->input);
		data->input = display_prompt();
		if (data->input == NULL)
		{
			write(2, "exit\n", 5);
			break ;
		}
		if (*(data->input) == '\0')
			continue ;
		table = parse(data->input);
		get_table_reset(table, 0);
		add_history(data->input);
		if (table)
		{
			execute(table);
			free_table(get_table_reset(NULL, 0));
		}
		get_table_reset(NULL, 1);
	}
	return (get_data(NULL)->last_ex_code);
}

// runs minishell in non-interactive, returns last exit code
int	run_non_interactive(char **argv)
{
	t_cmd_table	*table;

	table = parse(argv[2]);
	if (!table)
		malloc_error("ERROR: get_table");
	get_table_reset(table, 0);
	execute(table);
	return (get_data(NULL)->last_ex_code);
}

// returns 0 on interactive, returns 1 on non-interactive, exits on syntax-error
int	check_argv(int argc, char **av)
{
	if (argc == 1)
		return (0);
	if (check_cmd_option(av[1], 'c') == 1)
	{
		if (argc == 2)
			exit(bsh_err("minishell", av[1], "option requires an argument", 2));
	}
	else if (check_cmd_option(av[1], 'c') == -1)
		exit(bsh_err("minishell", av[1], "invalid option", 2));
	else
		exit(bsh_err("minishell", av[1], "command not found", 127));
	return (1);
}

/*
!FIXED!

!PATH - when deleted, now minishell acts like bash would
!getppid expelled from the project
!memcpy expelled from the project
!when open() fails, it shouldnt crash based on double free anymore - This should be rechecked just in case!
!exit -> keeps last_ex_code now
!exit -> prints now on stderr and not stdout
!when executable is empty it now prints (Permission denied; same as bash would)
!expansion doesnt occur with heredoc delimiter names anymore
!echo "$HOMER" problem fixed
!removing permission from parent folder no longer crashes heredoc execution (now its infinite hold; which is better I guess)
*new problem-> heredoc temp file isnt deleted properly when multiple heredocs present ->FIXED
*/


/*
Fixes to do
<<eof"" problem
*/
