
int builtin_check_table(t_cmd_table *table); // TODO switch to try_builtin
int builtin_check(char *input); // TODO switch to try_builtin
char *builtin_echo_parse_option(char *str); // TODO OLD not needed
int builtin_echo(char *input); // non-table version

int builtin_export_old(char **args); // TODO not finished
void test_execute(char* input, char **paths, char **envp); // TODO switch to execute()*/ 
int test_fork(char* exec_path, char **input_split, char **envp);

int key_search_remove(char *str);
/* int builtin_echo_option(char *str); // switched to check_cmd_option */


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

// TODO how to handle array_free_and_add fail?
// TODO export with no argument prints env? 
// TODO export abc def -> must add a string abc='' and def='' to env, not just abc and def
// returns 0 on SUCCESS, errno or 1 on Failure
int builtin_export_old(char **args)
{
	int i;

	DEBUG_printf("builtin_export");
	/* DEBUG_print_strings(split); */

	if (MAK_arr_size(args) == 1)
	{
		builtin_env(args);
		/* return (DEBUG_0("split size == 1, ran builtin_env\n")); */
		DEBUG_printf("split size == 1, ran builtin_env\n");
		return (0);
	}

	i = 1;
	while(args[i])
	{
		if (!array_free_and_add(&get_data(NULL)->mini_envp, args[i]))
		{
			DEBUG_printf("builtin_export: array_free_and failed\n");
			return (1); // TODO 1 okay? only fails on !arr !*arr !new_value
		}
		i++;
	}

	return (0);
}

// TODO move to DEL.c and switch to doma execute()
// path, arguments usage:
  /* char *const path = "/usr/bin/cat"; */
  /* char *const argv[] = {"/usr/bin/cat", "file.md", NULL}; */
// TODO get_env_paths in function instead of argument?
/* void test_execute(char* input, char **env_paths, char **envp) */
/* { */
/* 	char **input_split; */
/* 	char *exec_path; */

/* 	if (input == NULL || *input == '\0') */
/* 		return(DEBUG_VOID("test_execute: NULL or 0")); */
		
/* 	input_split = ft_split(input, ' '); */
/* 	if (input_split == NULL) */
/* 		malloc_error("ERROR: malloc failed in test_execute"); */
/* 	exec_path = return_executable_path(input_split[0], env_paths); */
/* 	/1* DEBUG_printf("ret: %s", test_ret); *1/ */
/* 	DEBUG_printf("INPUT: %s PATH: %s EXECUTABLE : %d\n", input, exec_path, */ 
/* 																is_executable(input_split[0], env_paths)); */

/* 	if (exec_path == NULL) */
/* 	{ */
/* 		free(exec_path); */
/* 		MAK_free_array(input_split); */
/* 		/1* DEBUG_printf("command not found"); *1/ */
/* 		perror("minishell: command not found"); */
/* 		return ; */
/* 	} */
/* 	free(input_split[0]); */
/* 	input_split[0] = ft_strdup(exec_path); */
/* 	/1* input_split[0] = exec_path; *1/ */

/* 	// TODO test_fork test */
/* 	test_fork(exec_path, input_split, envp); */

/* 	free(exec_path); */
/* 	MAK_free_array(input_split); */

/*   /1* if (execve(exec_path, input_split, envp) == -1) { *1/ */
/*   /1*     perror("execve failed"); *1/ */
/*   /1*     return ; *1/ */
/*   /1* } *1/ */
/* 	return; */
/* } */

int test_fork(char* exec_path, char **input_split, char **envp)
{
	pid_t pid = fork();

	if (pid < 0)
	{
		perror("fork");
		// TODO free
		exit(1);
	} 
	else if (pid == 0)
	{
		// Child process: run the command
  	if (execve(exec_path, input_split, envp) == -1) {
  	    perror("execve failed");
  	    return (1);
  	}
		// ???
		perror("execve"); // If execve returns, it failed
		/* exit(1); */
		return (1);
	} 
	else 
	{
		// Parent process: wait for the child
		int status;
		waitpid(pid, &status, 0);
	}
	return (0);
}

// TODO CRAP, already in unset, still needed?
int key_search_remove(char *str)
{
	int search;

	DEBUG_printf("key_search_remove str: %s", str);
	search = search_array(get_data(NULL)->mini_envp, str);
	if (search > 0)
	{
		if (!array_free_and_remove(&get_data(NULL)->mini_envp, str))
		{
			DEBUG_printf("key_search_remove: array_free_and_remove failed\n");
			return (1); // TODO 1 okay? only returns on !arr !*arr !remove_value, not found, mult. found
		}
	}

	return (0);
}

// S_IXUSR User has permission
// S_ISDIR is directory
// S_ISREG regular file
// S_ISREG(Stat.st_mode) && (Stat.st_mode & S_IXUSR) uses bitwise operator for permission
// TODO check file path too long?
// TODO ??? check S_IXUSR if user has file permission?
char *return_executable_path(const char *str, char **paths)
{
	int		i;
	char	*slash_path;
	char	*full_path;
	struct stat Stat;

	if (!str || !paths || !paths[0]) {
		return (DEBUG_NULL("!str || !paths || !paths[0]"));
	}
	
	// TODO root '/' should be executable?
	if (str[0] == '/' && ft_strlen(str) == 1) {
		return (DEBUG_NULL("is_executable: '/'"));
	}

	// check absolute str '/usr/bin/cat' and return copy
	if (stat(str, &Stat) == 0)
	{
		if (S_ISREG(Stat.st_mode) && (Stat.st_mode & S_IXUSR))
		{
			full_path = ft_strdup(str);
			if (!full_path)
				malloc_error("ERROR: malloc failed in return_executable_path: ft_strdup");
			return (full_path);
		}
	}

	// check relative str './minishell' TODO not needed?
		/* full_path = is_relative_path(str); */
		/* if(full_path) */
		/* 	if (stat(str, &Stat) == 0) */
	/* 		if (S_ISREG(Stat.st_mode) && (Stat.st_mode & S_IXUSR)) */
	/* 			return (full_path); */

	// If str is not an absolute path, check in PATH directories
	i = 0;
	while (paths[i])
	{
		slash_path = ft_strjoin(paths[i++], "/");
		if (!slash_path) {
			malloc_error("ERROR: malloc failed in return_executable_path: ft_strjoin");
		}
		full_path = ft_strjoin(slash_path, str);
		if (!full_path) {
			free(slash_path);
			malloc_error("ERROR: malloc failed in return_executable_path: ft_strjoin");
		}
		free(slash_path);

		if (stat(full_path, &Stat) == 0)
			if (S_ISREG(Stat.st_mode) && (Stat.st_mode & S_IXUSR))
				return(full_path);
		free(full_path);
	}
	return (NULL);
}

// NOT NEEDED
// S_IXUSR User has permission
// S_ISDIR is directory
// S_ISREG regular file
// S_ISREG(Stat.st_mode) && (Stat.st_mode & S_IXUSR) uses bitwise operator for permission
// TODO check file path too long?
// TODO ??? check S_IXUSR if user has file permission?
int is_executable(const char *str, char **paths)
{
	int		i;
	char	*slash_path;
	char	*full_path;
	struct stat Stat;

	if (!str || !paths || !paths[0]) {
		return (DEBUG_0("!str || !paths || !paths[0]"));
	}
	
	// TODO root '/' should be executable?
	if (str[0] == '/' && ft_strlen(str) == 1) {
		return (DEBUG_0("is_executable: '/'"));
	}

	// check absolute str '/usr/bin/cat'
	if (stat(str, &Stat) == 0)
		if (S_ISREG(Stat.st_mode) && (Stat.st_mode & S_IXUSR))
			return (1);

	// check relative str './minishell' TODO not needed?
	/* full_path = is_relative_path(str); */
	/* if(full_path) */
	/* 	if (stat(str, &Stat) == 0) */
	/* 		if (S_ISREG(Stat.st_mode) && (Stat.st_mode & S_IXUSR)) */
	/* 		{ */
	/* 			free(full_path); */
	/* 			return (1); */
	/* 		} */

	// If str is not an absolute path, check in PATH directories
	i = 0;
	while (paths[i])
	{
		slash_path = ft_strjoin(paths[i++], "/");
		if (!slash_path) {
			malloc_error("ERROR: malloc failed in is_executable: ft_strjoin");
		}
		full_path = ft_strjoin(slash_path, str);
		if (!full_path) {
			free(slash_path);
			malloc_error("ERROR: malloc failed in is_executable: ft_strjoin");
		}
		free(slash_path);

		if (stat(full_path, &Stat) == 0)
			if (S_ISREG(Stat.st_mode) && (Stat.st_mode & S_IXUSR))
			{
				free(full_path);
				return(1);
			}
		free(full_path);
	}
	return (0);
}

// TODO switched to check_cmd_option
// returns 1 for -n -nnnn.., 0 for -nnnnx etc
/* int bultin_echo_option(char *str) */
/* { */
/* 	int i; */

/* 	if (!str) */
/* 		return (0); */
/* 	if (ft_strlen(str) < 2) */
/* 		return (0); */
/* 	if (ft_strncmp(str, "-n", 2) != 0) */
/* 		return (0); */
/* 	i = 1; */
/* 	while(str[i]) */
/* 	{ */
/* 		if (str[i] != 'n') */
/* 			return (0); */
/* 		i++; */
/* 	} */
/* 	return(1); */
/* } */