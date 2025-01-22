#include "minishell.h"

// returns 0 on interactive, returns 1 on non-interactive, exits on syntax-error
int check_argv(int argc, char **argv)
{
	/* printf("\n argv:\n"); */
	/* DEBUG_print_strings(argv); */

	DEBUG_printf("is cmd_option: %d\n", check_cmd_option(argv[1], 'c'));

	if (argc == 1)
		return(0);
	if (check_cmd_option(argv[1], 'c') == 1)
	{
		if (argc == 2)
			exit(bash_error_msg("minishell", argv[1], "option requires an argument", 2));
	}
	else if (check_cmd_option(argv[1], 'c') == -1)
		exit(bash_error_msg("minishell", argv[1], "invalid option", 2));
	else
		exit(bash_error_msg("minishell", argv[1], "command not found", 127));

	return (1);
}

// TODO Not finished
// TODO Error Code Handling and ret not done yet
int run_non_interactive(char **argv)
{
	int ret;
	t_cmd_table *table;

	DEBUG_printf("\n----- NON-INTERACTIVE Minishell DEBUG -----");

	// TODO while( echo test; pwd; ls .. ) splits by ; ? table->next?
	// {
		table = parse(argv[2]);
			if (!table)
				malloc_error("ERROR: get_table");
		get_table_reset(table, 0);

		ret = execute(table);
	// }

	/* get_data(NULL)->last_ex_code = ret; // TODO not needed without loop */
	/* ret = data->last_ex_code; // TODO not needed without loop */
	DEBUG_printf("\n\tEXIT_STATUS ret: %d\n", ret);
	DEBUG_printf("\n\tEXIT_STATUS lst_ex: %d\n", get_data(NULL)->last_ex_code);
	free_all();

	(void)ret;
	exit (get_data(NULL)->last_ex_code);
	/* exit (ret); */

	/* return(ret); // TODO later run_interactive and run_non just return to main, and main returns */
}

int	main(int argc, char **argv, char **envp)
{
	char *input;
	int ret;
	t_data *data;
	t_cmd_table *table;

	/* key_value_tests(); */
	/* bash_error_tests(); */
	/* exit(0); */

	if (!envp)
		return(DEBUG_0("no envp"));

	int non_interactive_mode = 0;
	/* if (argc > 1) */
	/* { */
	/* 	shell_mode = check_argv(argc, argv); */
	/* 	DEBUG_printf("check_argv: %d", shell_mode); */
	/* } */

	non_interactive_mode = check_argv(argc, argv);
	DEBUG_printf("check_argv, is non_interactive: %d", non_interactive_mode);

	data = init_data(envp);
	get_data(data);

	if (non_interactive_mode == 1)
		run_non_interactive(argv);

  /* signal(SIGINT, signal_handler); */
  /* signal(SIGQUIT, signal_handler); */

	DEBUG_printf("\n-----INTERACTIVE Minishell DEBUG -----");
	DEBUG_printf("home: %s\n", get_home_path(envp));

	/* funcheck_tests(); // TODO DEBUG funcheck */
																										 //
	input = NULL;
	while(!input || data->exit_called == 0) // TODO check for "     exit"
	{
		free(input); // TODO readline return must be freed?
		input = display_prompt();
		if (*input == '\0')
			continue;

		table = parse(input);
		if (!table)
			malloc_error("ERROR: get_table");
		get_table_reset(table, 0);

		/* DEBUG_print_strings(table->args); // TODO DEBUG */

		DEBUG_printf("table->cmd: %s\n", table->cmd);

		add_history(input);

		ret = execute(table);

		// TODO only way test builtins in minishell-tester? 
		/* ret = try_builtin(table); */
		/* if(ret == -1) */
		/* 	execute(table); // doma execute */
		/* else */
		/* 	data->last_ex_code = ret; // TODO how to handle last_exit_status from execve? */

		free_table(get_table_reset(NULL, 0));
		get_table_reset(NULL, 1); // needs reset, or resource can't be declared NULL again
	}
	free(input); // TODO readline return must be freed?



	ret = data->last_ex_code;
	DEBUG_printf("\n\tEXIT_STATUS: %d\n", ret);
	free_all();
	return (ret);
	/* return (0); */
}







