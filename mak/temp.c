// TEMP.C
#include "../minishell.h"

t_data *get_data(t_data *data)
{
	static t_data *resource = NULL;

	if (data)
		resource = data;
	return (resource);
}

// TODO Error message? use exit_error?
t_data *init_data(char **envp)
{
	t_data *data;

	data = (t_data *)malloc(sizeof(t_data) * 1);
	if (!data)
		exit(EXIT_FAILURE);
	data->prompt_str = NULL;
	data->env_paths = NULL;
	data->home_path = NULL;
	data->original_envp = NULL;
	data->mini_envp = NULL;

	data->mini_envp = copy_array(envp);

	data->env_paths = get_envp_path(envp);
	if (data->env_paths == NULL)
	{
		free_data();
		return (0);
	}

	// TODO can home_path be NULL? How to guard?
	data->home_path = get_home_path(envp);
	if (data->home_path == NULL)
	{
		free_data();
		return (0);
	}

	data->original_envp = envp;

	return (data);
}

void free_data()
{
	t_data *data;

	data = get_data(NULL);
	if(data->prompt_str)
		free(data->prompt_str);
	if(data->env_paths)
		MAK_free_array(data->env_paths);
	// if(data->home_path)
	// 	free(data->home_path);
	free(data);
}

int	MAK_free_array(char **arr)
{
	size_t	i;

	if (!arr)
		return (0);
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (0);
}

// TODO use EXIT_FAILURE and perror instead
void exit_error(char *s)
{
	/* perror(s); */
	printf("%s\n", s);
	free_data();
	/* exit(EXIT_FAILURE); */
	exit(0);
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

	// No Color
	/* prompt_len = ft_strlen(user) + ft_strlen(hostname) + ft_strlen(cwd) + 4; */

	prompt_str = prepare_prompt_string(user, 
			cwd, hostname, prompt_len);

	return (readline(prompt_str));
}

char *prepare_prompt_string(char *user, char *path, char *hostname, int size)
{
	char *str;

	str = (char *)ft_calloc(sizeof(char), size + 1);
	if (!str)
		exit_error("prepare: str malloc failed");

	// write str to data for easier freeing
	// TODO prompt_str needs to be freed every time prompt is rewritten
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
// TODO getenv NULL -> exit or use (null)?
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

// TODO exit on getcwd Error?
char *get_cwd_path()
{
	char cwd[PATH_MAX];
	char *ret;
	int i;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		exit_error("ERROR: getcwd failed\n");
	ret = (char *)malloc(sizeof(char) * (ft_strlen(cwd) + 1));
	i = 0;
	while(cwd[i])
	{
		ret[i] = cwd[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

char **get_envp_path(char **envp)
{
	int     i;
	char    **paths;
	
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':'); // Skip "PATH="
	if (paths == NULL)
		return (NULL);
	return (paths);
}

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
		exit_error("is_relative_path ft_strjoin failed");
		/* return (NULL); */

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
			// TODO how to free?
			exit_error("!slash_path");
		}
		full_path = ft_strjoin(slash_path, str);
		if (!full_path) {
			// TODO how to free?
			free(slash_path);
			exit_error("!full_path");
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

	// check absolute str '/usr/bin/cat'
	if (stat(str, &Stat) == 0)
		if (S_ISREG(Stat.st_mode) && (Stat.st_mode & S_IXUSR))
			return (ft_strdup(str));

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
			// TODO how to free?
			exit_error("!slash_path");
		}
		full_path = ft_strjoin(slash_path, str);
		if (!full_path) {
			// TODO how to free?
			free(slash_path);
			exit_error("!full_path");
		}
		free(slash_path);

		if (stat(full_path, &Stat) == 0)
			if (S_ISREG(Stat.st_mode) && (Stat.st_mode & S_IXUSR))
				return(full_path);
		free(full_path);
	}
	return (NULL);
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
	else if (ft_strncmp(split[0], "env", 3) == 0 && split[0][3] == '\0')
		builtin_env();
	else if (ft_strncmp(split[0], "export", 6) == 0 && split[0][6] == '\0')
		builtin_export("test");
	/* else if (ft_strncmp(split[0], "exit", x) == 0) */
	/* else if (ft_strncmp(split[0], "export", x) == 0) */
	/* else if (ft_strncmp(split[0], "unset", x) == 0) */
	else // TODO else statement needed?
	{
		MAK_free_array(split);
		return (0);
	}

	MAK_free_array(split);
	return (1);
}


// TODO save new dir in data
int builtin_chdir(char **split)
{
	char *path;

	DEBUG_printf("bultin: cd");
	if (split == NULL)
		return(DEBUG_0("builtin_chdir split == NULL"));

	if (MAK_arr_size(split) > 2)
	{
		printf("minishell: cd: too many arguments\n");
		return (0);
	}

	if (MAK_arr_size(split) == 1)
		path = get_data(NULL)->home_path;
	else if (ft_strlen(split[1]) == 1 && split[1][0] == '~')
		path = get_data(NULL)->home_path;
	else
		path = split[1];

	if (chdir(path) != 0)
		perror("minishell: cd"); // 'minishell: cd: No such file or directory
	else // DEBUG
		DEBUG_printf("Directory changed to: %s\n", path);

	return (0);
}

int builtin_pwd(char **split)
{
	char *cwd_path;

	DEBUG_printf("bultin_pwd");
	if (MAK_arr_size(split) > 1)
	{
		printf("pwd: too many arguments\n");
		return (1);
	}

	cwd_path = get_cwd_path();
	printf("%s\n", cwd_path);
	free(cwd_path);

	return (0);
}

void builtin_env()
{
	char **env;
	env = get_data(NULL)->original_envp; // TODO change to mini_envp
	while(*env != NULL)
	{
		printf("%s\n", *env);
		env++;
	}
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

// Does not free input arr !
// TODO input entry should be malloced/freed or not?
// TODO handle strdup fail
// TODO use ft_calloc?
// TODO what if *arr / arr[0] is NULL or size 0?
char **add_to_array(char **arr, char *new_value)
{
	char **new_arr;
	int size;
	int i;

	if (!arr)
	{
		DEBUG_printf("no arr");
		return(NULL);
	}
	// if !new_value
	if (!new_value)
	{
		DEBUG_printf("no new_value");
		return(arr);
	}


	size = MAK_arr_size(arr);
	new_arr = malloc(sizeof(char *) * (size + 2));
	if (!new_arr)
		exit_error("copy_array malloc");

	i = 0;
	while(arr[i])
	{
		new_arr[i] = ft_strdup(arr[i]);
		if (!new_arr[i])
		{
    	while (--i >= 0)
    		free(new_arr[i]);
    	free(new_arr);
    	exit_error("add_to_array ft_strdup");
    }
		i++;
	}
	new_arr[i] = ft_strdup(new_value); // TODO refactor "free_failed_malloc"
	if (!new_arr[i])
	{
  	while (--i >= 0)
  		free(new_arr[i]);
  	free(new_arr);
  	exit_error("add_to_array ft_strdup");
  }

	new_arr[++i] = NULL;

	/* free(arr); */
	return (new_arr);
}

// TODO handle strdup fail
// TODO use ft_calloc?
// TODO what if *arr / arr[0] is NULL or size 0?
char **copy_array(char **arr)
{
	char **new_arr;
	int size;
	int i;

	if (!arr)
	{
		DEBUG_printf("no arr");
		return(NULL);
	}

	size = MAK_arr_size(arr);

	new_arr = malloc(sizeof(char *) * (size + 1));
	if (!new_arr)
		exit_error("copy_array malloc");

	i = 0;
	while(arr[i])
	{
		new_arr[i] = ft_strdup(arr[i]);
		if (!new_arr[i])
		{
    	while (--i >= 0)
    		free(new_arr[i]);
    	free(new_arr);
    	exit_error("copy_array ft_strdup");
    }
		i++;
	}
	new_arr[i] = NULL;
	return (new_arr);
}

/*
 *char **envp_copy = malloc((count + 1) * sizeof(char *));
    if (!envp_copy) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Copy each string
    for (int i = 0; i < count; i++) {
        envp_copy[i] = strdup(envp[i]);
        if (!envp_copy[i]) {
            perror("strdup");
            exit(EXIT_FAILURE);
        }
    }
    envp_copy[count] = NULL;  // Null-terminate the array

    return envp_copy;
 *
 * */


char *builtin_export(char *str)
{
	DEBUG_printf("bultin_echo");
	(void)str;
	return 0;
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
		exit_error("test_execute input_split"); // TODO how to handle?
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
	test_fork(exec_path, input_split, envp); // TODO change to mini_envp

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

//////////////////// TEMP ////////////////////
//////////////////// TEMP ////////////////////
//////////////////// TEMP ////////////////////


/* void stat_test(char *path) */
/* { */
/* 	struct stat Stat; */
/* 	int result; */

/* 	// 0 on success and -1 on failure */
/* 	result = stat(path, &Stat); */
/* 	if (result == -1) { */
/* 		perror("Error:"); */
/* 		exit(2); */
/* 	} */

/* 	//Test if Directory or not */
/* 	// > 0 means it is a directory */
/* 	if(S_ISDIR(Stat.st_mode) > 0 ){ */
/* 		printf("is a directory \n"); */
/* 	} */
	
/* 	//Test if its a regular file */
/* 	// > 0 means it is a directory */
/* 	if(S_ISREG(Stat.st_mode) > 0 ){ */
/* 		printf("is a regular file \n"); */
/* 	} */

/* 	if (result == 0) */
/* 		printf("path exists with result %d\n", result); */
/* 	else */
/* 		printf("path DOESNT exist with result %d\n", result); */
/* } */



















