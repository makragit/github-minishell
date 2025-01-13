
int builtin_check_table(t_cmd_table *table); // TODO switch to try_builtin
int builtin_check(char *input); // TODO switch to try_builtin
char *builtin_echo_parse_option(char *str); // TODO OLD not needed
int builtin_echo(char *input); // non-table version


// TODO Switch to try_builtin
int builtin_check_table(t_cmd_table *table)
{
	DEBUG_printf("bultin_check_table");
	if (!table)
		return (DEBUG_0("builtin_check_table: !table\n"));


	if (ft_strncmp(table->cmd, "cd", 2) == 0 && table->cmd[2] == '\0')
		builtin_chdir(table->args);
	else if (ft_strncmp(table->cmd, "echo", 4) == 0 && table->cmd[4] == '\0')
		builtin_echo_table(table->args);
		/* builtin_echo(table->args); */

/* DEBUG_printf("strncmp: %d\n", ft_strncmp(table->cmd, "echo", ft_strlen(table->cmd) == 0)); */
/* else if (ft_strncmp(table->cmd, "echo", ft_strlen(table->cmd) == 0)) */
				// TODO not working for echo 1 "2 3" 4 ? not executing if

	else
		return (0);

	return (1);
}

int builtin_check(char *input)
{
	char **split;

	if (*input == '\0' || input == NULL)
		return (DEBUG_0("builtin_check: 0 or NULL"));

	split = ft_split(input, ' ');
	if (split == NULL)
		exit_error("test_execute split"); // TODO how to handle?

	/* DEBUG_print_strings(split); */
	/* printf("\n"); */

	// TODO Refactor!
	if (ft_strncmp(split[0], "cd", 2) == 0 && split[0][2] == '\0')
		builtin_chdir(split);
	else if (ft_strncmp(split[0], "echo", 4) == 0 && split[0][4] == '\0')
		builtin_echo(input + 4);
	else if (ft_strncmp(split[0], "pwd", 3) == 0 && split[0][3] == '\0')
		builtin_pwd(split);
	/* else if (ft_strncmp(split[0], "env", 3) == 0 && split[0][3] == '\0') */
		/* builtin_env(); */
	else if (ft_strncmp(split[0], "export", 6) == 0 && split[0][6] == '\0')
		builtin_export(split);
	else if (ft_strncmp(split[0], "unset", 5) == 0 && split[0][5] == '\0')
		builtin_unset(split);
	/* else if (ft_strncmp(split[0], "exit", x) == 0) */

	else // TODO else statement needed?
	{
		MAK_free_array(split);
		return (0);
	}

	MAK_free_array(split);
	return (1);
}


// TODO echo 1 "2 3" 4 -> 1 2 3 4
// 					minishell 1"23"4
// TODO echo NO ARGUMENTS -> " " empty line
// TODO return/print output as whole string?
int builtin_echo(char *input)
{
	int i;
	char **split;
	/* int option_found; */
	char* option_found;

	option_found = NULL;
	(void)option_found;

	DEBUG_printf("bultin_echo");
	DEBUG_printf("NEEDS PARSE/LEXER");
	DEBUG_printf("input: _%s_", input);

	// char *builtin_echo_parse_option(char *str)
	option_found = builtin_echo_parse_option(input);
	if (option_found == NULL)
		split = ft_split(input, ' '); // TODO parse/lexer
		/* split = ft_split(input, '"'); // TODO parse/lexer */
	else 
		split = ft_split(option_found, ' '); // TODO parse/lexer
		/* split = ft_split(option_found, '"'); // TODO parse/lexer */

	i = 0;
	while(split[i])
	{
		printf("%s", split[i]);
		i++;
	}

	if (!option_found) // if -n found no newline
		printf("\n");

	/* (void)split; */
	MAK_free_array(split);
	return (0);
}



// TODO Not Needed anymore !!!
// TODO !!! ECHO needs parser/lexer for '""' etc
// Needs to split ' ' for whitespace and "2 3"  and '' correctly
char *builtin_echo_parse_option(char *str)
{
	while(ft_isspace(*str))
		str++;

	if (ft_strncmp(str, "-n", 2) != 0)
		return (DEBUG_NULL("NULL: no -n \n"));

	str += 2;
	while (*str == 'n')
		str++;

	if (!ft_isspace(*str) && *str != '\0')
		return (DEBUG_NULL("!isspace != 0 \n"));

	while(ft_isspace(*str))
		str++;
	
	DEBUG_printf("-n found");
	return (str);
}


// TODO handle strdup fail
// TODO use ft_calloc?
// TODO what if *arr / arr[0] is NULL or size 0?
/* Takes malloced array, frees it and creates new array without remove_value */

// int array_free_and_remove_2(char ***arr, char *remove_value)
// {
// 	char **new_arr;
// 	int size;
// 	int i;
// 
// 	if (!arr || !*arr)
//     return(DEBUG_0("array_free_remove: arr or *arr is NULL"));
// 	if (!remove_value)
//     return(DEBUG_0("array_free_remove: new_value is NULL"));
// 
// 	size = MAK_arr_size(*arr);
// 	new_arr = malloc(sizeof(char *) * (size + 2));
// 	if (!new_arr)
// 		exit_error("array_free_remove: malloc");
// 
// 	i = 0;
// 	while((*arr)[i])
// 	{
// 		if (ft_strncmp((*arr)[i], remove_value, ft_strlen(remove_value)) == 0)
// 		{
// 			(*arr)++;
// 			continue;
// 		}
// 		new_arr[i] = ft_strdup((*arr)[i]);
// 		if (!new_arr[i])
// 		{
// 			MAK_free_array(new_arr);
//     	exit_error("array_free_remove: ft_strdup");
//     }
// 		i++;
// 	}
// 
// 	/* new_arr[i] = ft_strdup(new_value); */
// 	/* if (!new_arr[i]) */
// 	/* { */
// 	/* 	MAK_free_array(new_arr); */
//   	/* exit_error("add_to_array ft_strdup"); */
//   /* } */
// 
// 	new_arr[i] = NULL;
// 	MAK_free_array(*arr);
// 	*arr = new_arr;
// 
// 	return (1);
// }






// TODO NOT NEEDED, use array_free_malloc_and_add instead
// Does not free input arr !
// TODO input entry should be malloced/freed or not?
// TODO handle strdup fail
// TODO use ft_calloc?
// TODO what if *arr / arr[0] is NULL or size 0?
/* char **add_to_array(char **arr, char *new_value) */
/* { */
/* 	char **new_arr; */
/* 	int size; */
/* 	int i; */

/* 	if (!arr) { */
/* 		DEBUG_printf("no arr"); */
/* 		return(NULL); */
/* 	} */
/* 	if (!new_value) 	{ */
/* 		DEBUG_printf("no new_value"); */
/* 		return(arr); */
/* 	} */

/* 	size = MAK_arr_size(arr); */
/* 	new_arr = malloc(sizeof(char *) * (size + 2)); */
/* 	if (!new_arr) */
/* 		exit_error("copy_array malloc"); */

/* 	i = 0; */
/* 	while(arr[i]) */
/* 	{ */
/* 		new_arr[i] = ft_strdup(arr[i]); */
/* 		if (!new_arr[i]) */
/* 		{ */
/*     	while (--i >= 0) */
/*     		free(new_arr[i]); */
/*     	free(new_arr); */
/*     	exit_error("add_to_array ft_strdup"); */
/*     } */
/* 		i++; */
/* 	} */
/* 	new_arr[i] = ft_strdup(new_value); // TODO refactor "free_failed_malloc" */
/* 	if (!new_arr[i]) */
/* 	{ */
/*   	while (--i >= 0) */
/*   		free(new_arr[i]); */
/*   	free(new_arr); */
/*   	exit_error("add_to_array ft_strdup"); */
/*   } */

/* 	new_arr[++i] = NULL; */

/* 	/1* free(arr); *1/ */
/* 	return (new_arr); */
/* } */