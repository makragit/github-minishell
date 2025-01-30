//////////////////// DEBUG ////////////////////
//////////////////// DEBUG ////////////////////
//////////////////// DEBUG ////////////////////

char	*mak_fetch_test(int counter)
{
	const char	*test[] =
	{
		// TEST TEST

		"echo hello",
		/* "echo", */
		/* "echo -n", */
		/* "echo hello", */
		/* "echo -n hello", */
		/* "pwd", */
		/* "env", */
		/* "ls", */
		/* "echo hello", */

		// Redirection errors
		"export EMPTY_VAR='' && echo $EMPTY_VAR", // SEG FAULT
		"export TEST=hello && echo $TEST && unset TEST",
		"export PATH=$PATH:/bin && echo $PATH",
		"unset PATH && echo $PATH",
		"export VAR=test && echo $VAR && unset VAR",
		"export TEST=test && echo \"Mixed $TEST string\"",
		"export TEST=\"multiline value\" && echo $TEST",  // SEG FAULT
		NULL

	};

	return ((char *)test[counter]);
}

void	funcheck_tests(t_data *data)
{
	int			counter;
	t_cmd_table	*table;
	int			ret;
	/* char *input_temp = NULL; */

	counter = 0;
	while (get_data(NULL)->exit_called == 0)
	{
		free(data->input);

		data->input = ft_strdup(fetch_test(counter));
		/* data->input = ft_strdup(mak_fetch_test(counter)); */
		if (data->input == NULL)
			break ;

		/* printf("Test Number: %i ON test: %s\n", counter, mak_fetch_test(counter)); */
		printf("Test Number: %i ON test: %s\n", counter, fetch_test(counter));
		counter++;

		table = parse(data->input);
		if (!table)
			malloc_error("ERROR: get_table");
		get_table_reset(table, 0);

		/* add_history(input); // TODO add_history has leaks and cant be guarded for Funcheck ???? */

		execute(table);
		free_table(get_table_reset(NULL, 0));
		get_table_reset(NULL, 1); // needs reset, or resource can't be declared NULL again
	}

	ret = get_data(NULL)->last_ex_code;
	free_all();
	printf("\nFUNCHECK TESTS DONE\n");
	exit (ret);
}

void	DEBUG_is_executable(char **paths)
{
	char	*commands[] = {"/", "/usr/bin/cat", "cat", "/nix", "/usr/nix",
		"usr/bin/cat", "/usr/bin/cat/", "./minishell", "./test-script/val-test.sh",
		"./notexisting", NULL};

	for (int i = 0; commands[i] != NULL; i++)
	{
		/* char *commands[i] = commands[i]; */
		/* DEBUG_printf("is_exec %s : %d\n", commands[i], is_executable(commands[i], paths)); */
		/* char *cmd = commands[i]; */
		/* DEBUG_printf("is_exec %s : %d\n", cmd, is_executable(cmd, paths)); */
	}

	(void)paths;
}

void	DEBUG_print_strings(char **arr)
{
	if (arr == NULL)
		return ;
	for (int i = 0; arr[i] != NULL; i++)
		printf("_%s_\n", arr[i]);
}

void	DEBUG_key_value_tests(void)
{
	printf("\n");
	printf("KEY VALUE TESTS\n");
	printf("env_key_valid %s : %d\n", "KEY", env_key_valid("KEY"));
	printf("env_key_valid %s : %d\n", "KEY=", env_key_valid("KEY="));
	printf("env_key_valid %s : %d\n", "1KEY=", env_key_valid("1KEY="));
	printf("env_key_valid %s : %d\n", "_KEY=", env_key_valid("_KEY="));
	printf("env_key_valid %s : %d\n", "_1KEY=", env_key_valid("_1KEY="));
	printf("env_key_valid %s : %d\n", "=", env_key_valid("="));

	printf("env_key_valid %s : %d\n", "MY_VAR", env_key_valid("MY_VAR"));
	printf("env_key_valid %s : %d\n", "123VAR", env_key_valid("123VAR"));
	printf("env_key_valid %s : %d\n", "_MYVAR", env_key_valid("_MYVAR"));
	printf("env_key_valid %s : %d\n", "MY_VAR_123", env_key_valid("MY_VAR_123"));
	printf("env_key_valid %s : %d\n", "MY-VAR", env_key_valid("MY-VAR"));
	printf("env_key_valid %s : %d\n", "MY VAR", env_key_valid("MY VAR"));
	printf("env_key_valid %s : %d\n", "MY=VAR", env_key_valid("MY=VAR"));

	printf("env_key_valid %s : %d\n", "abc", env_key_valid("abc"));
	printf("env_key_valid %s : %d\n", "ABC", env_key_valid("ABC"));
	printf("\n");
}

void	DEBUG_bash_error_tests(void)
{
	int	ret;

	printf("\n");
	ret = bsh_err("command", "arg", "message", 1);
	ret = bsh_err("command", NULL, "message", 1);
	ret = bsh_err("export", NULL, "message", 1);
	ret = bsh_err("unset", NULL, "message", 1);
	ret = bsh_err("export", "1KEY", "message", 1);
	ret = bsh_err("unset", "1KEY", "message", 1);
	printf("ret: %d\n", ret);

	printf("\n");

}
