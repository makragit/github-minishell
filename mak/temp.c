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

	data->exit_called = 0;

	get_data(data); // put here so copy_array free_data() can work on malloc fail

	data->original_envp = envp;
	data->mini_envp = copy_array(envp);
	if (data->mini_envp == NULL) 
    exit_error("ERROR: empty envp"); // TODO needed? envp is check in main
	data->env_paths = get_envp_path(data->mini_envp); // NULL if PATH= not in env variables
	data->home_path = get_home_path(data->mini_envp); // TODO not needed / updated. use get_home_path()
																										// "(null)" if no HOME= env varible

	return (data);
}

void free_data()
{
	t_data *data;

	rl_clear_history(); // TODO TEST

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
	char *prompt_str;
	int prompt_len;
	char *user;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		exit_error("ERROR: getcwd failed\n");

	/* user = getenv("USER"); */
	user = getenv_local("USER");
	if (user == NULL)
		user = "(null)";

	prompt_len = ft_strlen("\033[1;36m") + ft_strlen(user) + ft_strlen(cwd) + ft_strlen("\033[0m") + 4;

	prompt_str = prepare_prompt_string(user, cwd, prompt_len);

	return (readline(prompt_str));
}

char *prepare_prompt_string(char *user, char *prompt_path, int size)
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
	/* ft_strlcat(str, "@", size + 1); */
	/* ft_strlcat(str, hostname, size + 1); */
	ft_strlcat(str, ":", size + 1);
	ft_strlcat(str, prompt_path, size + 1);

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

	user = getenv_local("USER");
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
	DEBUG_printf("try_builtin");
	if (!table)
	{
		DEBUG_printf("try_builtin: !table\n");
		return (-1); // TODO -1 okay here? 1 only if execution fails
	}


	if (ft_strncmp(table->cmd, "cd", 2) == 0 && table->cmd[2] == '\0')
	{
		connect_pipes(table);
		redir(table);
		return (builtin_chdir(table->args));
	}
	else if (ft_strncmp(table->cmd, "echo", 4) == 0 && table->cmd[4] == '\0')
	{
		connect_pipes(table);
		redir(table);
		return (builtin_echo(table->args));
	}
	else if (ft_strncmp(table->cmd, "pwd", 3) == 0 && table->cmd[3] == '\0')
	{
		connect_pipes(table);
		redir(table);
		return (builtin_pwd(table->args));
	}
	else if (ft_strncmp(table->cmd, "env", 3) == 0 && table->cmd[3] == '\0')
	{
		connect_pipes(table);
		redir(table);
		return (builtin_env(table->args));
	}
	else if (ft_strncmp(table->cmd, "export", 6) == 0 && table->cmd[6] == '\0')
	{
		connect_pipes(table);
		redir(table);
		return (builtin_export(table->args));
	}
	else if (ft_strncmp(table->cmd, "unset", 5) == 0 && table->cmd[5] == '\0')
	{
		connect_pipes(table);
		redir(table);
		return (builtin_unset(table->args));
	}
	else if (ft_strncmp(table->cmd, "exit", 4) == 0 && table->cmd[4] == '\0')
	{
		connect_pipes(table);
		redir(table);
		return (builtin_exit(table->args));
	}

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
		return(bash_error_msg("cd", NULL, "too many arguments", 1));

	if (MAK_arr_size(args) == 1)
		path = get_home_path(get_data(NULL)->mini_envp);
		/* path = get_data(NULL)->home_path; */
	else if (ft_strlen(args[1]) == 1 && args[1][0] == '~')
		path = get_home_path(get_data(NULL)->mini_envp);
		/* path = get_data(NULL)->home_path; */
	else
		path = args[1];

	if (chdir(path) != 0)
	{
		/* perror("minishell: cd"); // 'minishell: cd: No such file or directory */
		/* return(1); // TODO INFO set to (1) instead of errno, */
							 // because errno returns 2 on 'not found' and 20 on 'not a dir'
							 // while bash exit code ist just 1
		/* return(errno); */
		return(bash_error_msg("cd", NULL, "No such file or directory", 1));
	}

	DEBUG_printf("Directory changed to: %s\n", path);

	return (0);
}

// TODO args not needed, remove from func(args)
int builtin_pwd(char **args)
{
	char *cwd_path;

	DEBUG_printf("bultin_pwd");

	/* if (MAK_arr_size(args) > 1) */
	/* { */
	/* 	return(bash_error_msg("pwd", NULL, "too many arguments", 1)); */
	/* } */
	(void)args;

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
		return(bash_error_msg("env", args[1], "No such file or directory", 0));

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

	DEBUG_printf("bultin_echo");

	if (!args || !*args)
	{
		DEBUG_printf("builtin_echo: !args !*args");
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
/* 0 on fail 1 success ; exit on malloc error*/
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
// FIX: Searches remove_value Key, removes all instances of remove_value and creates new array
int array_free_and_remove(char ***arr, char *remove_value)
{
	char **new_arr;
	int i;
	int found;

	char **temp_arr;
	temp_arr = (*arr);

	if (!arr || !*arr)
    return(DEBUG_0("array_free_remove: arr or *arr is NULL"));
	if (!remove_value)
    return(DEBUG_0("array_free_remove: new_value is NULL"));

	found = search_key_in_array((*arr), remove_value);

	// TODO DEBUG
	if (search_key_in_array((*arr), remove_value) > 1) // TODO can be deleted or integrated into if above
		DEBUG_0("array_free_and_remove: remove_value found multiple times");
	// TODO DEBUG

	if (!search_key_in_array((*arr), remove_value))
		return(DEBUG_0("array_free_and_remove: remove_value not found"));

	new_arr = malloc(sizeof(char *) * (MAK_arr_size(*arr) - found + 1));
													// size of array, minus values to be removed, +1 for NULL
	if (!new_arr)
		malloc_error("ERROR: malloc failed in array_free_and_remove");

	i = 0;
	while(*temp_arr)
	{
		if (ft_strncmp(*temp_arr, remove_value, len_to_equal_sign(remove_value)) != 0)
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

// returns how many times 'search' was found using in array
// New: now only compares until '='
// TODO Rename search_key_array
int search_key_in_array(char **arr, char *search)
{
	int i;
	int found;

	found = 0;
	i = 0;
	while(arr[i])
	{
		/* if (ft_strncmp(arr[i], search, ft_strlen(search)) == 0) */
		/* if (ft_strncmp(arr[i], search, len_to_eq) == 0) */
		if (ft_strncmp(arr[i], search, len_to_equal_sign(search)) == 0)
				found++;
		i++;
	}

	return (found);
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

		if (env_key_valid(args[i]) == 3) // 3 does not end with =
		{
			/* printf("minishell: unset: invalid parameter name"); // TODO perror? error_func */
			/* 																										// No Error message in bash! */
			i++;
			continue;
		}


		// TODO search_key_in_array for value to be removed
		search = search_key_in_array(get_data(NULL)->mini_envp, args[i]);
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

// returns 0 if invalid
// returns 1 if valid and without '=' at the end
// returns 2 if valid and WITH '=' at the end
// returns 3 not ending with '=' (abc=test)
int	env_key_valid(char *str)
{
	int	i;

	// check first letter (cant be digit, etc)
	if (!str)
		return (0);
	if (str[0] == '=')
		return (0);

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
			// 	OR if [0] after TEST= is '=' put quotes in

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

// TODO printf "exit" in builtin okay?
int builtin_exit(char **args)
{
	int ret;
	get_data(NULL)->exit_called = 1;

	/* DEBUG_print_strings(args); */

	if (MAK_arr_size(args) == 1)
	{
		printf("exit\n");
		return (errno);
	}

	if (MAK_arr_size(args) > 2)
	{
		get_data(NULL)->exit_called = 0; // 'too many args' does not exit
		printf("exit\n");
		return(bash_error_msg("exit", args[1], "too many arguments", 1));
	}

	if (!is_numeric(args[1]))
	{
		printf("exit\n");
		return(bash_error_msg("exit", args[1], "numeric argument required", 2));
	}

	ret = ft_atoi(args[1]);
	ret = (unsigned char)ret; // -100 to 156
	printf("exit\n");

	return (ret);
}

// check if a whole string is numeric
int is_numeric(const char *str)
{
	if (!str || *str == '\0') // Null or empty string check
		return (0);
	
	if (*str == '-' || *str == '+')
		str++;
	
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1); // All characters are digits
}


// TODO use unsigned int for error code so 100 -> 156
int bash_error_msg(char *cmd, char *arg, char *err_msg, int error_code)
{
	char *ret_msg;
	int size;
	int quotes;

	// TODO check for `KEY' quotes like this okay?
	quotes = 0;
	if (arg && ft_strncmp(cmd, "export", 4) == 0)
		quotes = 1;
	if (arg && ft_strncmp(cmd, "unset", 4) == 0)
		quotes = 1;
	if (arg && ft_strncmp(cmd, "env", 4) == 0)
		quotes = 1;

	size = 0;
	size = ft_strlen("minishell: ");
	size += ft_strlen(cmd);
	size += ft_strlen(err_msg);
	size += 2;

	if (arg)
	{
		size += ft_strlen(arg);
		size += 2;
	}
	if (quotes)
		size += 2;

	ret_msg = (char *)ft_calloc(sizeof(char), size + 1);
	if (!ret_msg)
		malloc_error("ERROR: malloc failed in bash_error_msg");
	
	ret_msg[size] = '\0';
	ft_strlcat(ret_msg, "minishell: ", size + 1);
	ft_strlcat(ret_msg, cmd, size + 1);
	ft_strlcat(ret_msg, ": ", size + 1);
	if (arg)
	{
		if (quotes) // quotes for `KEY'
			ft_strlcat(ret_msg, "`", size + 1);

		ft_strlcat(ret_msg, arg, size + 1);

		if (quotes) // quotes for `KEY'
			ft_strlcat(ret_msg, "'", size + 1);

		ft_strlcat(ret_msg, ": ", size + 1);
	}
	ft_strlcat(ret_msg, err_msg, size + 1);

	// print error
	ft_putendl_fd(ret_msg, 2); // STDERR_FILENO is 2 ?
	free(ret_msg);
	
	return (error_code);

}


// // TODO how to handle array_free_and_add fail?
// returns 0 on SUCCESS, errno or 1 on Failure
int builtin_export(char **args)
{
	int i;
	int ret;

	DEBUG_printf("builtin_export");
	/* DEBUG_print_strings(args); */
	DEBUG_printf("size: %d", MAK_arr_size(args));

	if (MAK_arr_size(args) == 1)
	{
		builtin_env(args);
		DEBUG_printf("split size == 0, ran builtin_env\n");
		return (0);
	}

	i = 1;
	while(args[i])
	{
		ret = export_handle_key_value(args, &i);
		if (args[i] != NULL) // i could be 2 now and increment too much
			i++;
	}
	return (ret);
}

// TODO how to handle array_free_and_add fail?
// TODO check error code return handling
// 0 invalid
// 1 ends without =
// 2 ends with =
// 3 not ending with = (abc=test)
int export_handle_key_value(char **args, int *i)
{
	int ret;
	char *key_value;

	DEBUG_printf("key_handling_args:");
	/* DEBUG_print_strings(args); */
	DEBUG_printf("\n");

	DEBUG_printf("args[*i]: %s\n", args[*i]);
	ret = env_key_valid(args[*i]);

	if (ret == 0) // invalid
		return(bash_error_msg("export", args[*i], "not a valid identifier", 1));

	if (search_key_in_array(get_data(NULL)->mini_envp, args[*i]) > 1) // TODO DEBUG
		DEBUG_0("export export_handle_key_value: more than 1 KEY found??"); // TODO DEBUG

	// If Key is valid, find all instancees of KEY currently in env and remove them
	array_free_and_remove(&get_data(NULL)->mini_envp, args[*i]); // Needs no 0 check

	// Add key and value, according to syntax and given arguments
	if (ret == 1 || (ret == 2 && args[*i+1] == NULL) ) // ends without =, or no possible value
	{
		key_value = ft_strjoin(args[*i], "=\'\'"); // VAL=""
		if (!key_value)
			malloc_error("ERROR: malloc failed in export_handle_key_value ft_strjoin");

		if (!array_free_and_add(&get_data(NULL)->mini_envp, key_value)) {
			DEBUG_printf("key_value_handling: array_free_and_add failed\n");
			free(key_value);
			return (1); // TODO 1 okay? only fails on !arr !*arr !new_value
		}
		free(key_value);
	}

	if (ret == 2) // ends with '='
	{
		key_value = ft_strjoin(args[*i], args[*i+1]);
		if (!key_value)
			malloc_error("ERROR: malloc failed in export_handle_key_value ft_strjoin");

		if (!array_free_and_add(&get_data(NULL)->mini_envp, key_value)) {
			DEBUG_printf("key_value_handling: array_free_and_add failed\n");
			free(key_value);
			return (1); // TODO 1 okay? only fails on !arr !*arr !new_value
		}
		free(key_value);
		(*i)++;
	}

	if (ret == 3) // not ending with '=' (test=abc)
	{	
		DEBUG_printf("[*i] ret==3: %d\n", *i);
		DEBUG_printf("args[*i] ret==3: %s\n", args[*i]);

		DEBUG_printf("search\n");
		DEBUG_printf("search: _%d_\n", search_key_in_array(get_data(NULL)->mini_envp, args[*i]));

		if (!array_free_and_add(&get_data(NULL)->mini_envp, args[*i])) {
			DEBUG_printf("key_value_handling: array_free_and_add failed\n");
			return (1); // TODO 1 okay? only fails on !arr !*arr !new_value
		}
	}

	return (0);
}

int len_to_equal_sign(char *str)
{
	int len;

	len = 0;
	while(str[len] && str[len] != '=')
		len++;

	return (len);
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





















