// TEMP.C
#include "../minishell.h"

t_data *get_data(t_data *data)
{
	static t_data *resource = NULL;

	if (data)
		resource = data;
	return (resource);
}

t_cmd_table *get_table_reset(t_cmd_table *table, int reset)
{
	static t_cmd_table *resource = NULL;
	
	if (reset)
		resource = NULL;
	else if (table != NULL)
		resource = table;
	return (resource);
}

// TODO Error message? use exit_error?
t_data *init_data(char **envp)
{
	t_data *data;

	data = (t_data *)malloc(sizeof(t_data) * 1);
	if (!data)
		malloc_error("ERROR: malloc failed in init_data");
		
	data->prompt_str = NULL;
	data->env_paths = NULL;
	data->home_path = NULL;
	data->original_envp = NULL;
	data->mini_envp = NULL;

	get_data(data); // put here so copy_array free_data() can work on malloc fail

	data->original_envp = envp;
	data->mini_envp = copy_array(envp);
	if (data->mini_envp == NULL) 
    exit_error("ERROR: empty envp"); // TODO needed? envp is check in main
	data->env_paths = get_envp_path(data->mini_envp); // NULL if PATH= not in env variables
	data->home_path = get_home_path(data->mini_envp); // "(null)" if no HOME= env variable

	return (data);
}

void free_data()
{
	t_data *data;

	data = get_data(NULL);
	if(!data)
		return;
	if(data->prompt_str)
		free(data->prompt_str);
	if(data->env_paths)
		MAK_free_array(data->env_paths);
	// if(data->home_path)
	// 	free(data->home_path);
	if(data->mini_envp)
		MAK_free_array(data->mini_envp);
	free(data);
}

void free_all()
{
	t_cmd_table *table;

	free_data();

	table = get_table_reset(NULL, 0);
	if (!table)
		return;
	else
		free_table(table);
}

int	MAK_free_array(char **arr)
{
	size_t	i;
if (!arr) return (0);
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (0);
}

// same as malloc_error for now
void exit_error(char *s)
{
	perror(s);
	free_all();

	if (errno == 0)
		errno = 1;

	exit(errno);
}

// TODO perror or stderror?
void malloc_error(char *s)
{
	perror(s);
	free_all();

	if (errno == 0)
		errno = 1;

	exit(errno);
}

// TODO guard bigger than PATH_MAX?
char *display_prompt()
{
	char cwd[PATH_MAX];
	char *hostname;
	char *prompt_str;
	int prompt_len;
	char *user;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		exit_error("ERROR: getcwd failed\n");

	user = getenv("USER");
	if (user == NULL)
		user = "(null)";

	hostname = getenv("HOSTNAME"); // TODO no HOSTNAME or HOST env variable
	if (hostname == NULL)
		hostname = "(null)";

	prompt_len = ft_strlen("\033[1;36m") + ft_strlen(user)
		+ ft_strlen(hostname) + ft_strlen(cwd) + ft_strlen("\033[0m") + 4;

	prompt_str = prepare_prompt_string(user, 
			cwd, hostname, prompt_len);

	return (readline(prompt_str));
}

char *prepare_prompt_string(char *user, char *path, char *hostname, int size)
{
	char *str;

	str = (char *)ft_calloc(sizeof(char), size + 1);
	if (!str)
		malloc_error("ERROR: malloc failed in prepare_prompt_string");

	free(get_data(NULL)->prompt_str);
	get_data(NULL)->prompt_str = str;

	str[size] = '\0';
	ft_strlcat(str, "\033[1;36m", size + 1);
	ft_strlcat(str, user, size + 1);
	ft_strlcat(str, "@", size + 1);
	ft_strlcat(str, hostname, size + 1);
	ft_strlcat(str, ":", size + 1);
	ft_strlcat(str, path, size + 1);

	if (is_root())
		ft_strlcat(str, "# ", size + 1);
	else
		ft_strlcat(str, "$ ", size + 1);
	ft_strlcat(str, "\033[0m", size + 1);

	return (str);
}

// TODO getenv for root okay? getuid() not allowed
int is_root()
{
	char *user;

	user = getenv("USER");
	if (user == NULL)
		user = "(null)";
		/* exit_error("is_root getenv NULL"); */
	if (ft_strncmp(user, "root", 4) == 0)
		return (1);
	return (0);
}

// TODO which whitespace to check for "    -n" ?
int ft_isspace(char c)
{
	if (c == ' ')
		return (1);
	/* if (c == '\f') */
	/* 	return (1); */
	/* if (c == '\n') */
	/* 	return (1); */
	/* if (c == '\r') */
	/* 	return (1); */
	/* if (c == '\t') */
	/* 	return (1); */
	/* if (c == '\v') */
	/* 	return (1); */
	return (0);
}

void signal_handler(int signum)
{
	if (signum == SIGINT)
	{
	    DEBUG_printf("\nReceived Ctrl-C (SIGINT). Use Ctrl-\\ to quit.\n");
			/* display_prompt(); // TODO input wont be read! t_data? */
	    exit(0);
	}
	else if (signum == SIGQUIT)
	{
	    DEBUG_printf("\nQuitting program due to Ctrl-\\ (SIGQUIT).\n");
	    exit(0);
	}
}

int	MAK_arr_size(char **arr)
{
	int i;

	if(!arr)
		return (0);

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

// returns malloced 'current working directory', exits program on getcwd() failure
char *get_cwd_path()
{
	char cwd[PATH_MAX];
	char *ret;
	int i;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		exit_error("ERROR: getcwd failed\n");
	ret = (char *)malloc(sizeof(char) * (ft_strlen(cwd) + 1));
	if (ret == NULL)
		malloc_error("ERROR: malloc failed in get_cwd_path");

	i = 0;
	while(cwd[i])
	{
		ret[i] = cwd[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

// returns array of split PATH= values, returns NULL if !envp or no PATH= found
char **get_envp_path(char **envp)
{
	int     i;
	char    **paths;

	if (!envp) {
		DEBUG_NULL("get_envp: no envp");
		return(NULL);
	}

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':'); // Skip "PATH="
	if (paths == NULL)
		malloc_error("ERROR: malloc failed in getenvp_path: ft_split");
	return (paths);
}

// Does not need to be freed
char *get_home_path(char **envp)
{
	char 	*home_dir;
	int		i;
	
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "HOME=", 5) != 0)
		i++;
	if (!envp[i])
	{
		DEBUG_NULL("No HOME: in envp");
		return ("(null)"); // return "(null)" because chdir can't handle NULL
	}

	home_dir = envp[i] + 5;
  // Check if the home directory is empty
  if (home_dir == NULL || *home_dir == '\0')
	{
  	DEBUG_NULL("HOME: is empty");
  	return ("(null)");
  }

	return (home_dir);
}

// NOT NEEDED
// TODO guard bigger than PATH_MAX?
/* char *resolve_relative_path(char *path) */
char *is_relative_path(const char *str)
{
	char cwd[PATH_MAX];
	char *full_path;
	struct stat Stat;

	if (str[0] != '.')
		return (NULL);
	if (str[1] != '/')
		return (NULL);

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		exit_error("ERROR: getcwd failed\n");

	full_path = ft_strjoin(cwd, str);
	if (!full_path)
		malloc_error("ERROR: malloc failed in is_relative_path: ft_strjoin");

	if (stat(str, &Stat) == 0)
		if (S_ISREG(Stat.st_mode) && (Stat.st_mode & S_IXUSR))
			return (full_path);

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


int try_builtin(t_cmd_table *table)
{
	DEBUG_printf("bultin_check_table");
	if (!table)
	{
		DEBUG_printf("builtin_check_table: !table\n");
		return (-1); // TODO -1 okay here? 1 only if execution fails
	}

	if (ft_strncmp(table->cmd, "cd", 2) == 0 && table->cmd[2] == '\0')
		return (builtin_chdir(table->args));
	else if (ft_strncmp(table->cmd, "echo", 4) == 0 && table->cmd[4] == '\0')
		return (builtin_echo(table->args));
	else if (ft_strncmp(table->cmd, "pwd", 3) == 0 && table->cmd[3] == '\0')
		return (builtin_pwd(table->args));
	else if (ft_strncmp(table->cmd, "env", 3) == 0 && table->cmd[3] == '\0')
		return (builtin_env(table->args));
	else if (ft_strncmp(table->cmd, "export", 6) == 0 && table->cmd[6] == '\0')
		return (builtin_export(table->args));
	else if (ft_strncmp(table->cmd, "unset", 5) == 0 && table->cmd[5] == '\0')
		return (builtin_unset(table->args));
	else if (ft_strncmp(table->cmd, "exit", 4) == 0 && table->cmd[4] == '\0')
		return (builtin_exit(table->args));

	return (-1);
}


// TODO save new dir in data
// returns 0 for success, errno or 1 on failure
int builtin_chdir(char **args)
{
	char *path;

	DEBUG_printf("bultin: cd");

	if (args == NULL)
	{
		DEBUG_0("builtin_chdir args == NULL");
		return(1);
	}

	if (MAK_arr_size(args) > 2)
	{
		printf("minishell: cd: too many arguments\n"); // TODO perror? error_func
		return (1);
	}

	if (MAK_arr_size(args) == 1)
		path = get_data(NULL)->home_path;
	else if (ft_strlen(args[1]) == 1 && args[1][0] == '~')
		path = get_data(NULL)->home_path;
	else
		path = args[1];

	if (chdir(path) != 0)
	{
		perror("minishell: cd"); // 'minishell: cd: No such file or directory
		return(1); // TODO INFO set to (1) instead of errno,
							 // because errno returns 2 on 'not found' and 20 on 'not a dir'
							 // while bash exit code ist just 1
		/* return(errno); */
	}

	DEBUG_printf("Directory changed to: %s\n", path);

	return (0);
}

int builtin_pwd(char **args)
{
	char *cwd_path;

	DEBUG_printf("bultin_pwd");
	if (MAK_arr_size(args) > 1)
	{
		printf("pwd: too many arguments\n");
		return (1);
	}

	cwd_path = get_cwd_path();
	printf("%s\n", cwd_path);
	free(cwd_path);

	return (0);
}

// returns 0 on SUCCESS, errno or 1 on Failure
int builtin_env(char **args)
{
	char **env;

	DEBUG_printf("bultin: env");
	if (MAK_arr_size(args) > 1)
	{
		printf("env: ‘%s‘: No such file or directory\n", args[1]);
		return (0); // TODO 0 correct? 
	}

	env = get_data(NULL)->mini_envp;
	while(*env != NULL)
	{
		printf("%s\n", *env);
		env++;
	}

	return (0);
}


// returns 1 for -n -nnnn.., 0 for -nnnnx etc
int bultin_echo_option(char *str)
{
	int i;

	if (!str)
		return (0);
	if (ft_strlen(str) < 2)
		return (0);
	if (ft_strncmp(str, "-n", 2) != 0)
		return (0);
	i = 1;
	while(str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return(1);
}

// TODO multiple printfs okay for output?
// return 0 on Success, errno or 1 on Failure
int builtin_echo(char **args)
{
	int i;
	int option_found = 0;

	DEBUG_printf("bultin_echo_table");

	if (!args || !*args)
	{
		DEBUG_printf("builtin_echo_table: !args !*args");
		return (1);
	}
	if (MAK_arr_size(args) == 1)
	{
		printf("\n");
		return (0);
	}

	DEBUG_printf("echo_parse_option: %d\n", bultin_echo_option(args[1]));
	if (bultin_echo_option(args[1]))
		option_found = 1;

	i = 1 + option_found; // if -n found: skip the string

	if (args[i]) // first value without space
		printf("%s", args[i++]);
	while(args[i])
	{
		printf(" %s", args[i]); // uses space
		i++;
	}

	if (!option_found) // if -n found no newline
		printf("\n");

	return (0);
}

// TODO use ft_calloc?
// TODO what if *arr / arr[0] is NULL or size 0?
/* Takes malloced array, frees it and creates new array with added Value using the same ptr*/
/* 0 on fail 1 success */
int array_free_and_add(char ***arr, char *new_value)
{
	char **new_arr;
	int size;
	int i;

	if (!arr || !*arr)
    return(DEBUG_0("array_free_and_add: arr or *arr is NULL"));
	if (!new_value)
    return(DEBUG_0("array_free_and_add: new_value is NULL"));

	size = MAK_arr_size(*arr);
	new_arr = malloc(sizeof(char *) * (size + 2));
	if (!new_arr)
		malloc_error("ERROR: malloc failed in array_free_and_add");

	i = 0;
	while((*arr)[i])
	{
		new_arr[i] = ft_strdup((*arr)[i]);
		if (!new_arr[i])
		{
			MAK_free_array(new_arr);
			malloc_error("ERROR: malloc failed in array_free_and_add: ft_strdup");
    }
		i++;
	}
	new_arr[i] = ft_strdup(new_value);
	if (!new_arr[i])
	{
		MAK_free_array(new_arr);
		malloc_error("ERROR: malloc failed in array_free_and_add: ft_strdup");
  }

	new_arr[++i] = NULL;
	MAK_free_array(*arr);
	*arr = new_arr;

	return (1);
}

// TODO use ft_calloc?
// removes value from array. Only works if value is unique in array (like in envp)
int array_free_and_remove(char ***arr, char *remove_value)
{
	char **new_arr;
	int size;
	int i;

	char **temp_arr;
	temp_arr = (*arr);

	if (!arr || !*arr)
    return(DEBUG_0("array_free_remove: arr or *arr is NULL"));
	if (!remove_value)
    return(DEBUG_0("array_free_remove: new_value is NULL"));

	// Search for remove_value and return if not found
	if (!search_array((*arr), remove_value))
		return(DEBUG_0("array_free_and_remove: remove_value not found"));
	if (search_array((*arr), remove_value) > 1) // TODO can be deleted or integrated into if above
		return(DEBUG_0("array_free_and_remove: remove_value found multiple times"));

	size = MAK_arr_size(*arr);
	new_arr = malloc(sizeof(char *) * (size)); // excluding +1 because of removed value
	if (!new_arr)
		malloc_error("ERROR: malloc failed in array_free_and_remove");

	i = 0;
	while(*temp_arr)
	{
		if (ft_strncmp(*temp_arr, remove_value, ft_strlen(remove_value)) != 0)
		{
			new_arr[i] = ft_strdup(*temp_arr);
			if (!new_arr[i++])
			{
				MAK_free_array(new_arr);
				malloc_error("ERROR: malloc failed in array_free_and_remove: ft_strdup");
    	}
		}

		temp_arr++;
	}

	new_arr[i] = NULL;
	MAK_free_array(*arr);
	*arr = new_arr;

	return(1);
}



// TODO use ft_calloc?
// TODO what if *arr / arr[0] is NULL or size 0?
char **copy_array(char **arr)
{
	char **new_arr;
	int size;
	int i;

	if (!arr || !*arr) {
		DEBUG_printf("copy_array: !arr !*arr");
		return(NULL);
	}

	size = MAK_arr_size(arr);

	new_arr = malloc(sizeof(char *) * (size + 1));
	if (!new_arr)
		malloc_error("ERROR: malloc failed in copy_array");

	i = 0;
	while(arr[i])
	{
		new_arr[i] = ft_strdup(arr[i]);
		if (!new_arr[i])
		{
			MAK_free_array(new_arr);
			malloc_error("ERROR malloc failed in copy_array: ft_strdup");
    }
		i++;
	}
	new_arr[i] = NULL;
	return (new_arr);
}

// TODO check ft_strlen of arr[i] instead for exact match?
// returns how many times 'search' was found using in array
// search is not exact, only checks for len of 'search'
int search_array(char **arr, char *search)
{
	int i;
	int found;

	found = 0;
	i = 0;
	while(arr[i])
	{
		if (ft_strncmp(arr[i], search, ft_strlen(search)) == 0)
				found++;
		i++;
	}

	return (found);
}

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

// TODO unset behaviour multiple arguments? Deletes all?
// TODO unset behaviour value not found?
// TODO unset behaviour multiple arguments, one value not found?
// TODO Refactor if-else
// returns 0 on SUCCESS, errno or 1 on Failure
int builtin_unset(char **args)
{
	int i;
	int search;

	DEBUG_printf("builtin_unset");

	if (MAK_arr_size(args) == 1)
		return (0);
		/* printf("unset: not enough arguments\n"); // no Error message in bash */


	i = 1;
	while(args[i])
	{

		if (key_valid(args[i]) == 3) // 3 does not end with =
		{
			/* printf("minishell: unset: invalid parameter name"); // TODO perror? error_func */
			/* 																										// No Error message in bash! */
			i++;
			continue;
		}


		// TODO search_array for value to be removed
		search = search_array(get_data(NULL)->mini_envp, args[i]);
		if (search < 1) {
			DEBUG_printf("bultin_unset: search not found");
			i++;
			continue;
		}
		// TODO DEBUG check > 1
		if (search > 1) {
			DEBUG_printf("bultin_unset: SEARCH found > 1 ??");
			i++;
			continue;
		}

		if (!array_free_and_remove(&get_data(NULL)->mini_envp, args[i]))
		{
			DEBUG_printf("builtin_export: array_free_and_remove failed\n");
			return (1); // TODO 1 okay? only returns on !arr !*arr !remove_value, not found, mult. found
		}
		i++;
	}

	return (0);
}

// path, arguments usage:
  /* char *const path = "/usr/bin/cat"; */
  /* char *const argv[] = {"/usr/bin/cat", "file.md", NULL}; */
// TODO get_env_paths in function instead of argument?
void test_execute(char* input, char **env_paths, char **envp)
{
	char **input_split;
	char *exec_path;

	if (input == NULL || *input == '\0')
		return(DEBUG_VOID("test_execute: NULL or 0"));
		
	input_split = ft_split(input, ' ');
	if (input_split == NULL)
		malloc_error("ERROR: malloc failed in test_execute");
	exec_path = return_executable_path(input_split[0], env_paths);
	/* DEBUG_printf("ret: %s", test_ret); */
	DEBUG_printf("INPUT: %s PATH: %s EXECUTABLE : %d\n", input, exec_path, 
																is_executable(input_split[0], env_paths));

	if (exec_path == NULL)
	{
		free(exec_path);
		MAK_free_array(input_split);
		/* DEBUG_printf("command not found"); */
		perror("minishell: command not found");
		return ;
	}
	free(input_split[0]);
	input_split[0] = ft_strdup(exec_path);
	/* input_split[0] = exec_path; */

	// TODO test_fork test
	test_fork(exec_path, input_split, envp);

	free(exec_path);
	MAK_free_array(input_split);

  /* if (execve(exec_path, input_split, envp) == -1) { */
  /*     perror("execve failed"); */
  /*     return ; */
  /* } */
	return;
}

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

// returns 0 if invalid
// returns 1 if valid and without '=' at the end
// returns 2 if valid and WITH '=' at the end
// returns 3 not ending with '=' (abc=test)
int	key_valid(char *str)
{
	int	i;

	// check first letter (cant be digit, etc)
	if (!str)
		return (0);
	if (str[0] == '=')
		return (0);

	/* if ((str[0] <= 'A' || str[0] >= 'Z') */
	/* 	&& (str[0] <= 'a' || str[0] >= 'z') */
	/* 	&& (str[0] != '_')) */
	/* 	return (0); */


	if (!((str[0] >= 'A' && str[0] <= 'Z')
		||(str[0] >= 'a' && str[0] <= 'z')
		||(str[0] == '_')))
		return (0);



	i = 1;
	while (str[i] != '\0' && str[i] != '=')
	{
		if ((str[i] >= 'A' && str[i] <= 'Z')
			|| (str[i] >= 'a' && str[i] <= 'z')
			|| (str[i] >= '0' && str[i] <= '9')
			|| (str[i] == '_'))
			i++;
		else
			return (0);
	}

	// TODO might have to change this for export ZZZ==test,
			// 	output must be ZZZ='=test' and table returns 'ZZZ==test'

	// check for potential case where '=' is not in the end
	if (str[i] == '=')
	{
			if (str[i + 1] != '\0')
				return (3);
			else
				return (2); // if key ends with '=' return 2
	}
	return (1);
}

// TODO needs to return correct error code back
// 0 invalid
// 1 ends without =
// 2 ends with =
// 3 not ending with = (abc=test)
int key_value_handling(char **args)
{
	int i;
	int ret;
	char *key_value;

	/* key_value = NULL; */

	i = 1; // start with 'export'

	DEBUG_printf("key_handling_args:");
	/* DEBUG_print_strings(args); */
	DEBUG_printf("\n");

	while(args[i])
	{
		ret = key_valid(args[i]);
		DEBUG_printf("key_value_handling arg: %s", args[i]);
		DEBUG_printf("key_value_handling key_valid ret: %d", ret);

		/* if (ret == 1) // ends without = */
		if (ret == 1 || (ret == 2 && args[i+1] == NULL) ) // ends without =, or no possible value
		{
			key_search_remove(args[i]); // TODO Unique check CRAP make better

			key_value = ft_strjoin(args[i], "=\'\'"); // VAL=""
			if (!array_free_and_add(&get_data(NULL)->mini_envp, key_value)) {
				DEBUG_printf("key_value_handling: array_free_and_add failed\n");
				return (1); // TODO 1 okay? only fails on !arr !*arr !new_value
			}
			free(key_value);
			/* key_value = NULL; */
		}

		if (ret == 2) // ends with =
		{

//			size = 0;
//			size = ft_strlen(args[i]); // TODO try size in one line
//			size += ft_strlen(value);
//			/* size += 2; // " and " */
//
//			key_value = malloc(sizeof(char *) * (size + 1));
//			if (!key_value)
//				malloc_error("ERROR: malloc failed in array_free_and_add");
//
//			key_value[size] = '\0';
//			ft_strlcat(key_value, args[i], size + 1);
//			/* ft_strlcat(key_value, "\"", size + 1); */
//			ft_strlcat(key_value, args[i+1], size + 1);
//			/* ft_strlcat(key_value, "\"", size + 1); */


			key_search_remove(args[i]); // TODO Unique check CRAP make better

			key_value = ft_strjoin(args[i], args[i+1]);

			if (!array_free_and_add(&get_data(NULL)->mini_envp, key_value)) {
				DEBUG_printf("key_value_handling: array_free_and_add failed\n");
				return (1); // TODO 1 okay? only fails on !arr !*arr !new_value
			}

			free(key_value);
			/* key_value = NULL; */
			i++; // skip next arg thats already used as value
		}

		if (ret == 3) // not ending with '=' (test=abc)
		{	
			key_search_remove(args[i]); // TODO Unique check CRAP make better

			if (!array_free_and_add(&get_data(NULL)->mini_envp, args[i])) {
				DEBUG_printf("key_value_handling: array_free_and_add failed\n");
				return (1); // TODO 1 okay? only fails on !arr !*arr !new_value
			}
		}

		if (ret == 0)
			printf("minishell: export: `(var)': not a valid identifier\n"); // TODO perror? error_func


		i++;
	}

	return (0);
}

// NEW NEW NEW
// NEW NEW NEW
// NEW NEW NEW
// TODO how to handle array_free_and_add fail?
// TODO export with no argument prints env? 
// TODO export abc def -> must add a string abc='' and def='' to env, not just abc and def
// returns 0 on SUCCESS, errno or 1 on Failure
int builtin_export(char **args)
{
	/* int i; */

	DEBUG_printf("builtin_export");
	/* DEBUG_print_strings(split); */

	if (MAK_arr_size(args) == 1)
	{
		builtin_env(args);
		/* return (DEBUG_0("split size == 1, ran builtin_env\n")); */
		DEBUG_printf("split size == 1, ran builtin_env\n");
		return (0);
	}


	int ret;
	ret = 0;
	ret = key_value_handling(args);

	return (ret);
}

int builtin_exit(char **args)
{
	int ret;

	if (MAK_arr_size(args) == 1)
		return (0); // TODO return 0 if exit without number argument?

	if (MAK_arr_size(args) > 2)
	{
		printf("minishell: exit: too many arguments\n"); // TODO perror? error_func
		return (1);
	}

	if (!is_numeric(args[1]))
	{
		printf("minishell: exit: numeric argument required\n"); // TODO perror? error_func
		return (2);
	}

	ret = ft_atoi(args[1]);

	return (ret);
}



int is_numeric(const char *str)
{
	if (!str || *str == '\0') // Null or empty string check
		return (0);
	
	if (*str == '-' || *str == '+')
		str++;
	
	while (*str)
	{
		/* if (!isdigit((unsigned char)*str)) // Check if the character is not a digit */
		if (!ft_isdigit(*str))
			return (0);
	str++;
	}
	return (1); // All characters are digits
}



//////////////////// DEBUG ////////////////////
//////////////////// DEBUG ////////////////////
//////////////////// DEBUG ////////////////////

void DEBUG_is_executable(char **paths)
{
	char *commands[] = {"/", "/usr/bin/cat", "cat", "/nix", "/usr/nix", 
		"usr/bin/cat", "/usr/bin/cat/", "./minishell", "./test-script/val-test.sh",
		"./notexisting", NULL};

	for (int i = 0; commands[i] != NULL; i++) {
		/* char *commands[i] = commands[i]; */
		DEBUG_printf("is_exec %s : %d\n", commands[i], is_executable(commands[i], paths));
		/* char *cmd = commands[i]; */
		/* DEBUG_printf("is_exec %s : %d\n", cmd, is_executable(cmd, paths)); */
	}

	(void)paths;
}

void DEBUG_print_strings(char **arr)
{
	if (arr == NULL) return;
	
	for (int i = 0; arr[i] != NULL; i++)
		printf("_%s_\n", arr[i]);
}





















