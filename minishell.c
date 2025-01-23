#include "minishell.h"

/* signal(SIGINT, signal_handler); */
/* signal(SIGQUIT, signal_handler); */

int	main(int argc, char **argv, char **envp)
{
	int ret;
	t_data *data;
	int non_interactive_mode;

	if (!envp)
		return(DEBUG_0("no envp"));

	/* DEBUG_print_strings(envp); */

	non_interactive_mode = check_argv(argc, argv);
	data = init_data(envp);
	get_data(data);

	/* funcheck_tests(data); // TODO DEBUG funcheck */

	if (non_interactive_mode == 1)
		ret = run_non_interactive(argv);
	else
		ret = run_interactive(data);


	free_all();
	return (ret);
}

// runs minishell in interactive, returns last exit code
int run_interactive(t_data *data)
{
	t_cmd_table *table;

	DEBUG_printf("\n-----INTERACTIVE Minishell DEBUG -----");

	while(data->exit_called == 0)
	{
		free(data->input);
		data->input = display_prompt();
		if (*(data->input) == '\0')
			continue;
		
		table = parse(data->input);
		if (!table)
			malloc_error("ERROR: get_table");
		get_table_reset(table, 0);

		add_history(data->input);
		execute(table);
		free_table(get_table_reset(NULL, 0));
		get_table_reset(NULL, 1); // needs reset, or resource can't be declared NULL again

	}
	DEBUG_printf("\n\tEXIT_STATUS: %d\n", data->last_ex_code);

	return(get_data(NULL)->last_ex_code);
}

// runs minishell in non-interactive, returns last exit code
int run_non_interactive(char **argv)
{
	t_cmd_table *table;

	DEBUG_printf("\n----- NON-INTERACTIVE Minishell DEBUG -----");

	table = parse(argv[2]);
		if (!table)
			malloc_error("ERROR: get_table");
	get_table_reset(table, 0);

	execute(table);

	DEBUG_printf("\n\tEXIT_STATUS :%d\n", get_data(NULL)->last_ex_code);

	return(get_data(NULL)->last_ex_code);
}

// returns 0 on interactive, returns 1 on non-interactive, exits on syntax-error
int check_argv(int argc, char **argv)
{
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



