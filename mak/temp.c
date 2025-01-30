#include "../minishell.h"

t_data	*get_data(t_data *data)
{
	static t_data	*resource = NULL;

	if (data)
		resource = data;
	return (resource);
}

t_cmd_table	*get_table_reset(t_cmd_table *table, int reset)
{
	static t_cmd_table	*resource = NULL;

	if (reset)
		resource = NULL;
	else if (table != NULL)
		resource = table;
	return (resource);
}

char	**create_empty_envp(void)
{
	char	**empty_envp;
	char	*cwd_path;

	empty_envp = malloc(sizeof(char *) * 2);
	if (!empty_envp)
		malloc_error("ERROR: malloc failed in init_data");
	cwd_path = get_cwd_path();
	empty_envp[0] = ft_strjoin("PWD=", cwd_path);
	if (!empty_envp[0])
	{
		free(cwd_path);
		free(empty_envp);
		malloc_error("ERROR: malloc failed in init_data");
	}
	empty_envp[1] = NULL;
	free(cwd_path);
	return (empty_envp);
}

t_data	*init_data(char **envp)
{
	t_data	*data;

	data = (t_data *)malloc(sizeof(t_data) * 1);
	if (!data)
		malloc_error("ERROR: malloc failed in init_data");
	ft_memset(data, 0, sizeof(t_data));
	get_data(data);
	data->mini_envp = copy_array(envp);
	if (data->mini_envp == NULL)
		data->mini_envp = create_empty_envp();
	return (data);
}

void	free_data(void)
{
	t_data	*data;

	rl_clear_history();
	data = get_data(NULL);
	if (!data)
		return ;
	if (data->prompt_str)
		free(data->prompt_str);
	if (data->last_cwd)
		free(data->last_cwd);
	if (data->input)
		free(data->input);
	if (data->env_paths)
		mak_free_array(data->env_paths);
	if (data->mini_envp)
		mak_free_array(data->mini_envp);
	free(data);
}

void	free_all(void)
{
	t_cmd_table	*table;

	free_data();
	table = get_table_reset(NULL, 0);
	if (!table)
		return ;
	else
		free_table(table);
}

int	mak_free_array(char **arr)
{
	size_t	i;

	if (!arr)
		return (0);
	i = 0;
	while (arr[i])
	{
		free (arr[i]);
		i++;
	}
	free(arr);
	return (0);
}

void	exit_error(char *s)
{
	perror(s);
	free_all();
	if (errno == 0)
		errno = 1;
	exit(errno);
}

void	malloc_error(char *s)
{
	perror(s);
	free_all();
	if (errno == 0)
		errno = 1;
	exit(errno);
}

char	*display_prompt(void)
{
	char	cwd[PATH_MAX];
	char	*user;
	int		prompt_len;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		if (get_data(NULL)->prompt_str == NULL)
			return (readline("(no current working directory): "));
		else
			return (readline(get_data(NULL)->prompt_str));
	}
	free(get_data(NULL)->last_cwd);
	get_data(NULL)->last_cwd = get_cwd_path();
	user = getenv_local("USER");
	if (user == NULL)
		user = "(null)";
	prompt_len = ft_strlen("\033[1;36m") + ft_strlen(user)
		+ ft_strlen(cwd) + ft_strlen("\033[0m") + 4;
	free(get_data(NULL)->prompt_str);
	get_data(NULL)->prompt_str = prepare_prompt_string(user, cwd, prompt_len);
	return (readline(get_data(NULL)->prompt_str));
}

char	*prepare_prompt_string(char *user, char *prompt_path, int size)
{
	char	*str;

	str = (char *)ft_calloc(sizeof(char), size + 1);
	if (!str)
		malloc_error("ERROR: malloc failed in prepare_prompt_string");
	str[size] = '\0';
	ft_strlcat(str, "\033[1;36m", size + 1);
	ft_strlcat(str, user, size + 1);
	ft_strlcat(str, ":", size + 1);
	ft_strlcat(str, prompt_path, size + 1);
	if (is_root())
		ft_strlcat(str, "# ", size + 1);
	else
		ft_strlcat(str, "$ ", size + 1);
	ft_strlcat(str, "\033[0m", size + 1);
	return (str);
}

int	is_root(void)
{
	char	*user;

	user = getenv_local("USER");
	if (user == NULL)
		return (0);
	if (ft_strncmp(user, "root", 4) == 0)
		return (1);
	return (0);
}

int	ft_isspace(char c)
{
	if (c == ' ')
		return (1);
	return (0);
}

int	mak_arr_size(char **arr)
{
	int	i;

	if (!arr)
		return (0);
	i = 0;
	while (arr[i])
		i++;
	return (i);
}

// returns malloced 'current working directory', 
// 		exits program on getcwd() failure
char	*get_cwd_path(void)
{
	char	cwd[PATH_MAX];
	char	*ret;
	int		i;
	char	*ret_null;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		ret_null = ft_strdup("(null)");
		if (!ret_null)
			malloc_error("ERROR: malloc failed in get_cwd_path: ft_strdup");
		return (ret_null);
	}
	ret = (char *)malloc(sizeof(char) * (ft_strlen(cwd) + 1));
	if (ret == NULL)
		malloc_error("ERROR: malloc failed in get_cwd_path");
	i = 0;
	while (cwd[i])
	{
		ret[i] = cwd[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

// returns array of split PATH= values, returns NULL if !envp or no PATH= found
char	**get_envp_path(char **envp)
{
	int		i;
	char	**paths;

	if (!envp)
		return (NULL);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	if (paths == NULL)
		malloc_error("ERROR: malloc failed in getenvp_path: ft_split");
	return (paths);
}

int	try_builtin(t_cmd_table *table)
{
	if (!table)
		return (-1);
	if (ft_strncmp(table->cmd, "cd", 2) == 0 && table->cmd[2] == '\0')
		return (builtin_chdir(table->args));
	else if (ft_strncmp(table->cmd, "echo", 4) == 0 && table->cmd[4] == '\0')
		return (builtin_echo(table->args));
	else if (ft_strncmp(table->cmd, "pwd", 3) == 0 && table->cmd[3] == '\0')
		return (builtin_pwd());
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

// returns 0 for success, errno or 1 on failure
int	builtin_chdir(char **args)
{
	char	*path;

	if (args == NULL)
		return (1);
	if (mak_arr_size(args) > 2)
		return (bsh_err("cd", NULL, "too many arguments", 1));
	if (mak_arr_size(args) == 1)
	{
		path = getenv_local("HOME");
		if (path == NULL)
			return (bsh_err("cd", NULL, "HOME not set", 1));
	}
	else if (ft_strlen(args[1]) == 1 && args[1][0] == '~')
	{
		path = getenv_local("HOME");
		if (path == NULL)
			return (bsh_err("cd", NULL, "HOME not set", 1));
	}
	else
		path = args[1];
	if (chdir(path) != 0)
		return (bsh_err("cd", NULL, "No such file or directory", 1));
	builtin_chdir_update_pwd();
	return (0);
}

void	builtin_chdir_update_pwd(void)
{
	char	*cwd_path;

	cwd_path = get_cwd_path();
	update_env_var("PWD=", cwd_path);
	free(cwd_path);
	return ;
}

int	builtin_pwd(void)
{
	char	*cwd_path;

	cwd_path = get_cwd_path();
	if (cwd_path == NULL)
	{
		if (get_data(NULL)->last_cwd == NULL)
			printf("(null)\n");
		else
			printf("%s\n", get_data(NULL)->last_cwd);
		return (0);
	}
	printf("%s\n", cwd_path);
	free(cwd_path);
	return (0);
}

// returns 0 on SUCCESS, errno or 1 on Failure
int	builtin_env(char **args)
{
	char	**env;

	if (mak_arr_size(args) > 1)
		return (bsh_err("env", args[1], "No such file or directory", 0));
	env = get_data(NULL)->mini_envp;
	while (*env != NULL)
	{
		printf("%s\n", *env);
		env++;
	}
	return (0);
}

// returns 1 for -n -nnnn.., 0 for -nnnnx, ----n etc
// returns -1 if '-' found but option invalid 
// 		(for '-c' check in non-interactive shell)
int	check_cmd_option(char *str, char option)
{
	int	i;

	if (!str)
		return (0);
	if (ft_strlen(str) < 2)
		return (0);
	if (str[0] != '-')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != option)
			return (-1);
		i++;
	}
	return (1);
}

// return 0 on Success, errno or 1 on Failure
int	builtin_echo(char **args)
{
	int	i;
	int	option_found;

	if (!args || !*args)
		return (1);
	if (mak_arr_size(args) == 1)
	{
		printf("\n");
		return (0);
	}
	option_found = 0;
	if (check_cmd_option(args[1], 'n'))
		option_found = 1;
	i = 1 + option_found;
	if (args[i])
		printf("%s", args[i++]);
	while (args[i])
	{
		printf(" %s", args[i]);
		i++;
	}
	if (!option_found)
		printf("\n");
	return (0);
}

/* Takes malloced array, frees it and creates new array 
 * with added Value using the same arr ptr*/
/* 0 on fail 1 success ; -1 on malloc error (to fix a free-leak */
int	array_free_and_add(char ***arr, char *new_value)
{
	char	**new_arr;
	int		size;

	if (!arr || !*arr)
		return (0);
	if (!new_value)
		return (0);
	size = mak_arr_size(*arr);
	new_arr = malloc(sizeof(char *) * (size + 2));
	if (!new_arr)
		return (-1);
	if (array_add_help(*arr, new_arr, new_value) == -1)
		return (-1);
	mak_free_array(*arr);
	*arr = new_arr;
	return (1);
}

int	array_add_help(char **arr, char **new_arr, char *new_v)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		new_arr[i] = ft_strdup(arr[i]);
		if (!new_arr[i])
		{
			mak_free_array(new_arr);
			return (-1);
		}
		i++;
	}
	new_arr[i] = ft_strdup(new_v);
	if (!new_arr[i])
	{
		mak_free_array(new_arr);
		return (-1);
	}
	new_arr[++i] = NULL;
	return (0);
}

// FIX: Searches remove_value Key, 
// removes all instances of remove_value and creates new array
int	array_free_and_rem(char ***arr, char *rem_value)
{
	char	**new_arr;
	int		found;
	char	**temp_arr;

	temp_arr = (*arr);
	if (!arr || !*arr || !rem_value)
		return (0);
	found = search_key_in_array((*arr), rem_value);
	if (!search_key_in_array((*arr), rem_value))
		return (0);
	new_arr = malloc(sizeof(char *) * (mak_arr_size(*arr) - found + 1));
	if (!new_arr)
		malloc_error("ERROR: malloc failed in array_free_and_rem");
	new_arr = array_rem_help(temp_arr, new_arr, rem_value);
	mak_free_array(*arr);
	*arr = new_arr;
	return (1);
}

char	**array_rem_help(char **arr, char **new_arr, char *rem)
{
	int	i;

	i = 0;
	while (*arr)
	{
		if (ft_strncmp(*arr, rem, len_to_equal_sign(rem)) != 0)
		{
			new_arr[i] = ft_strdup(*arr);
			if (!new_arr[i++])
			{
				mak_free_array(new_arr);
				malloc_error("ERROR: malloc fail in arr_rem_hlp: ft_strdp");
			}
		}
		arr++;
	}
	new_arr[i] = NULL;
	return (new_arr);
}

char	**copy_array(char **arr)
{
	char	**new_arr;
	int		size;
	int		i;

	if (!arr || !*arr)
		return (NULL);
	size = mak_arr_size(arr);
	new_arr = malloc(sizeof(char *) * (size + 1));
	if (!new_arr)
		malloc_error("ERROR: malloc failed in copy_array");
	i = 0;
	while (arr[i])
	{
		new_arr[i] = ft_strdup(arr[i]);
		if (!new_arr[i])
		{
			mak_free_array(new_arr);
			malloc_error("ERROR malloc failed in copy_array: ft_strdup");
		}
		i++;
	}
	new_arr[i] = NULL;
	return (new_arr);
}

// returns how many times 'search' was found using in array
// New: now only compares until '='
int	search_key_in_array(char **arr, char *search)
{
	int	i;
	int	found;

	found = 0;
	i = 0;
	while (arr[i])
	{
		if (ft_strncmp(arr[i], search, len_to_equal_sign(search)) == 0)
			found++;
		i++;
	}
	return (found);
}

// updates or creates ENV if not found
// 		NEEDS "PWD=" with '=' as key (for now)
void	update_env_var(char *key, char *value)
{
	int		search;
	int		i;
	char	*key_value;
	char	**temp_envp;

	if (!key || !value)
		return ;
	key_value = ft_strjoin(key, value);
	if (!key_value)
		malloc_error("update_env_var: malloc error");
	if (env_key_valid(key) != 2)
		return ;
	search = search_key_in_array(get_data(NULL)->mini_envp, key);
	if (search <= 0)
	{
		if (array_free_and_add(&get_data(NULL)->mini_envp, key_value) == -1)
			malloc_error("update_env_var: malloc error");
		return ;
	}
	if (search > 1)
		return ;
	temp_envp = get_data(NULL)->mini_envp;
	i = 0;
	while (temp_envp[i])
	{
		if (ft_strncmp(temp_envp[i], key, ft_strlen(key)) == 0)
		{
			free(temp_envp[i]);
			temp_envp[i] = ft_strdup(key_value);
			break ;
		}
		i++;
	}
	free(key_value);
	return ;
}

// returns 0 on SUCCESS, errno or 1 on Failure
int	builtin_unset(char **args)
{
	int	i;
	int	search;

	if (mak_arr_size(args) == 1)
		return (0);
	i = 1;
	while (args[i])
	{
		search = search_key_in_array(get_data(NULL)->mini_envp, args[i]);
		if (env_key_valid(args[i]) >= 2 || search < 1)
		{
			i++;
			continue ;
		}
		if (!array_free_and_rem(&get_data(NULL)->mini_envp, args[i]))
			return (1);
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

	if (!str || str[0] == '=')
		return (0);
	if (!((str[0] >= 'A' && str[0] <= 'Z')
			|| (str[0] >= 'a' && str[0] <= 'z')
			|| (str[0] == '_')))
		return (0);
	i = 1;
	while (str[i] != '\0' && str[i] != '=')
	{
		if (!env_key_valid_helper(str[i]))
			return (0);
		i++;
	}
	if (str[i] == '=')
	{
		if (str[i + 1] != '\0')
			return (3);
		else
			return (2);
	}
	return (1);
}

int	env_key_valid_helper(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9')
		|| (c == '_'))
		return (1);
	else
		return (0);
}

int	builtin_exit(char **args)
{
	int	ret;

	get_data(NULL)->exit_called = 1;
	if (mak_arr_size(args) == 1)
	{
		printf("exit\n");
		return (errno);
	}
	if (mak_arr_size(args) > 2)
	{
		get_data(NULL)->exit_called = 0;
		printf("exit\n");
		return (bsh_err("exit", args[1], "too many arguments", 1));
	}
	if (!is_numeric(args[1]))
	{
		printf("exit\n");
		return (bsh_err("exit", args[1], "numeric argument required", 2));
	}
	ret = ft_atoi(args[1]);
	ret = (unsigned char)ret;
	printf("exit\n");
	return (ret);
}

int	is_numeric(const char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	bsh_err(char *cmd, char *arg, char *msg, int code)
{
	char	*ret_msg;
	int		size;
	int		quotes;

	if (!cmd || !msg)
	{
		ft_putendl_fd("minishell: Error", 2);
		return (code);
	}
	quotes = (arg && (ft_strncmp(cmd, "export", 6) == 0
				|| ft_strncmp(cmd, "unset", 5) == 0
				|| ft_strncmp(cmd, "env", 3) == 0));
	size = ft_strlen("minishell: ") + ft_strlen(cmd) + ft_strlen(msg) + 2;
	if (arg)
		size += ft_strlen(arg) + 2 + quotes + quotes;
	ret_msg = bsh_err_help(cmd, arg, size, quotes);
	ft_strlcat(ret_msg, msg, size + 1);
	ft_putendl_fd(ret_msg, 2);
	free(ret_msg);
	return (code);
}

char	*bsh_err_help(char *cmd, char *arg, int size, int qte)
{
	char	*ret_msg;

	ret_msg = (char *)ft_calloc(sizeof(char), size + 1);
	if (!ret_msg)
		malloc_error("ERROR: malloc failed in bsh_err");
	ret_msg[size] = '\0';
	ft_strlcat(ret_msg, "minishell: ", size + 1);
	ft_strlcat(ret_msg, cmd, size + 1);
	ft_strlcat(ret_msg, ": ", size + 1);
	if (arg)
	{
		if (qte)
			ft_strlcat(ret_msg, "`", size + 1);
		ft_strlcat(ret_msg, arg, size + 1);
		if (qte)
			ft_strlcat(ret_msg, "'", size + 1);
		ft_strlcat(ret_msg, ": ", size + 1);
	}
	return (ret_msg);
}

// returns 0 on SUCCESS, errno or 1 on Failure
int	builtin_export(char **args)
{
	int	i;
	int	ret;

	if (mak_arr_size(args) == 1)
	{
		builtin_env(args);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		ret = export_handle_key_value(args, &i);
		if (args[i] != NULL)
			i++;
	}
	return (ret);
}

// 0 invalid
// 1 ends without =
// 2 ends with =
// 3 not ending with = (abc=test)
int	export_handle_key_value(char **args, int *i)
{
	int		ret;
	char	*key_value;

	ret = env_key_valid(args[*i]);
	if (ret == 0)
		return (bsh_err("export", args[*i], "not a valid identifier", 1));
	array_free_and_rem(&get_data(NULL)->mini_envp, args[*i]);
	key_value = export_format_key_value(args, i, ret);
	if (!key_value)
		malloc_error("ERROR: malloc fail expt_hndle_key strjoin");
	if (array_free_and_add(&get_data(NULL)->mini_envp, key_value) == -1)
	{
		free(key_value);
		malloc_error("exprt_hndle_key malloc array_free_and_add");
	}
	free(key_value);
	return (0);
}

// returns malloced formatted key_value
char	*export_format_key_value(char **args, int *i, int ret)
{
	char	*key_value;

	key_value = NULL;
	if (ret == 1 || (ret == 2 && !args[*i + 1]))
		key_value = ft_strjoin(args[*i], "=\'\'");
	else if (ret == 2)
	{
		key_value = ft_strjoin(args[*i], args[*i + 1]);
		(*i)++;
	}
	else if (ret == 3)
		key_value = ft_strdup(args[*i]);
	return (key_value);
}

int	len_to_equal_sign(char *str)
{
	int	len;

	len = 0;
	while (str[len] && str[len] != '=')
		len++;
	return (len);
}
