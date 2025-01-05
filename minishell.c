#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char *input;
	t_data *data;
	t_cmd_table *table;

	main_test(); // TEST DOMAGOJ

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

	while(!input || ft_strncmp(input, "exit", 5) != 0) // TODO check for "     exit"
	{
		free(input); // TODO readline return must be freed?
		input = display_prompt();
		if (*input == '\0')
			continue;
		/*
		t_cmd_table *table = cmd_table(input);
		if (!table)
			exit(1);

		// example ls -l
		char * table.cmd = "ls"
		char ** table.args = {ls, -l, NULL};
		char *table.cmd_path = "bin/etc/ls";
		
		//after executing
		free_table(table);
		table = NULL;
		*/

		table = parse(input);
		if (!table)
			exit(1); // TODO free_data + free_table

		DEBUG_print_strings(table->args);
		DEBUG_printf("table->cmd: %s\n", table->cmd);

		/* free_table(table); */
		/* table = NULL; */





		add_history(input);
		/* if(!builtin_check(input)) // not a builtin -> execve */
		if(!builtin_check_table(table)) // not a builtin -> execve
			test_execute(input, data->env_paths, data->mini_envp);
			/* test_execute(input, data->env_paths, envp); */
	}
	free(input); // TODO readline return must be freed?

	printf("exit\n");

	(void)argv; // TODO get rid of argv in main()?
	free_data();
	return (0);
}







