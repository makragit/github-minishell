#include "minishell.h"

void key_value_tests()
{
	printf("\n");
	printf("KEY VALUE TESTS\n");
	printf("key_valid %s : %d\n", "KEY", key_valid("KEY"));
	printf("key_valid %s : %d\n", "KEY=", key_valid("KEY="));
	printf("key_valid %s : %d\n", "1KEY=", key_valid("1KEY="));
	printf("key_valid %s : %d\n", "_KEY=", key_valid("_KEY="));
	printf("key_valid %s : %d\n", "_1KEY=", key_valid("_1KEY="));
	printf("key_valid %s : %d\n", "=", key_valid("="));

	printf("key_valid %s : %d\n", "MY_VAR", key_valid("MY_VAR"));
	printf("key_valid %s : %d\n", "123VAR", key_valid("123VAR"));
	printf("key_valid %s : %d\n", "_MYVAR", key_valid("_MYVAR"));
	printf("key_valid %s : %d\n", "MY_VAR_123", key_valid("MY_VAR_123"));
	printf("key_valid %s : %d\n", "MY-VAR", key_valid("MY-VAR"));
	printf("key_valid %s : %d\n", "MY VAR", key_valid("MY VAR"));
	printf("key_valid %s : %d\n", "MY=VAR", key_valid("MY=VAR"));

	printf("key_valid %s : %d\n", "abc", key_valid("abc"));
	printf("key_valid %s : %d\n", "ABC", key_valid("ABC"));
	printf("\n");
}

int	main(int argc, char **argv, char **envp)
{
	char *input;
	int ret;
	t_data *data;
	t_cmd_table *table;

	/* main_test(); // TEST DOMAGOJ */
	// now fetch_test?
	/* key_value_tests(); */
	/* exit(0); */

  signal(SIGINT, signal_handler);
  signal(SIGQUIT, signal_handler);

	DEBUG_printf("\n----- Minishell DEBUG -----");
	DEBUG_printf("home: %s\n", get_home_path(envp));

	if (argc != 1 || !envp)
		return (DEBUG_0("argc !=1 || !envp"));

	data = NULL;
	data = init_data(envp);
	get_data(data);

	/* DEBUG_is_executable(data->env_paths); */

	input = NULL;

	/* input = ft_strdup("exit"); 							 // TODO DEBUG funcheck */
	/* if (!input) 																 // TODO DEBUG funcheck */
	/* 	exit_error("main: ft_strdup malloc fail"); // TODO DEBUG funcheck */

	/* while(!input || ft_strncmp(input, "exit", 5) != 0) // TODO check for "     exit" */
	while(!input || ft_strncmp(input, "exit", 4) != 0) // TODO check for "     exit"
	{
		free(input); // TODO readline return must be freed?
		input = display_prompt();
		if (*input == '\0')
			continue;

		table = parse(input);
		if (!table)
			malloc_error("ERROR: get_table");
		get_table_reset(table, 0);

		/* DEBUG_print_strings(table->args); */
		DEBUG_printf("table->cmd: %s\n", table->cmd);

		ret = try_builtin(table);
		DEBUG_printf("try_builtin return: %d", ret);
		add_history(input);
		if(ret == -1)
			test_execute(input, data->env_paths, data->mini_envp);
		else
			data->last_exit_status = ret; // TODO how to handle last_exit_status from execve?


		free_table(get_table_reset(NULL, 0));
		get_table_reset(NULL, 1); // needs reset, or resource can't be declared NULL again
	}
	free(input); // TODO readline return must be freed?

	printf("exit\n");

	(void)argv; // TODO get rid of argv in main()?

	ret = data->last_exit_status;
	DEBUG_printf("\n\tEXIT_STATUS: %d\n", ret);
	free_all();
	return (ret);
	/* return (0); */
}







