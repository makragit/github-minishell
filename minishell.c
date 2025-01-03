#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char *input;
	t_data *data;

	main_test(); // TEST

  signal(SIGINT, signal_handler);
  signal(SIGQUIT, signal_handler);

	DEBUG_printf("\n----- Minishell DEBUG -----");
	DEBUG_printf("home: %s\n", get_home_path(envp));

	if (argc != 1 || !envp)
		return (DEBUG_0("argc !=1 || !envp"));

	data = init_data(envp);
	get_data(data);

	/* DEBUG_is_executable(data->env_paths); */

	input = NULL;
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

		example ls -l
		char * table.cmd = "ls"
		char ** table.args = {ls, -l, NULL};
		char *table.cmd_path = "bin/etc/ls";


		//	
		
		//after executing
		free_table(table);
		table = NULL;
		*/
		add_history(input);
		if(!builtin_check(input)) // not a builtin -> execve
			test_execute(input, data->env_paths, envp);
	}
	free(input); // TODO readline return must be freed?

	printf("exit\n");

	(void)argv; // TODO get rid of argv in main()?
	free_data();
	return (0);
}







