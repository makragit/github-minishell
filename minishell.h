#ifndef MINISHELL_H
# define MINISHELL_H

# include "./debug/debug.h" // TODO DEBUG
# include "libft/libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <linux/limits.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <signal.h>
# include <string.h>
# include <ctype.h>
# include <stdbool.h>
# include <errno.h>

typedef enum t_err
{
	OK,
	ERROR,
	FAIL,
	SINGLE_CMD,
	NO,
	EQUAL,
	ALL,
	ONE,
	S_QUOTE,
	CAPTURE,
	RESTORE,
}	t_err;

typedef enum t_token_type
{
	CMD_t,
	ARG_TOKEN,
	APPEND,
	HEREDOC,
	INPUT,
	REDIRECTION,
	OUTPUT,
	WORD,
	PIPE,
}	t_token_type;

typedef struct t_token
{
	char			*value;
	t_token_type	type;
	struct t_token	*next;
	struct t_token	*prev;
}	t_token;

typedef struct t_redir_data
{
	char					*input;
	char					*output;
	char					*append;
	char					*heredoc_delimiter;
	char                    *heredoc_file_name;
	int                      is_quoted;
	struct t_redir_data		*next;
	struct t_redir_data		*prev;
}	t_redir_data;

typedef struct t_cmd_table
{
	int					index;
	int					stdin_backup;
	int					stdout_backup;
	t_token				*tokens;
	char				*cmd;
	char				**args;
	t_redir_data		*redir_data;
	int					p_input_fd;
	int					p_output_fd;
	struct t_cmd_table	*next;
}	t_cmd_table;

typedef struct s_data
{
	int		last_ex_code;
	char	*prompt_str;
	char	**env_paths;
	char	*home_path;
	char	**original_envp;
	char	**mini_envp;

}	t_data;

// MAK
// temp.c
t_data *get_data(t_data *data);
t_cmd_table *get_table_reset(t_cmd_table *table, int reset);
t_data *init_data(char **envp);

void free_data(); // frees only data
void free_all(); // frees data and table
int	MAK_free_array(char **arr);

void exit_error(char *s); // TODO switch to malloc_error
void malloc_error(char *s);

char *display_prompt();
char *prepare_prompt_string(char *user, char *path, char *hostname, int size);

int is_root();
int ft_isspace(char c);
void signal_handler(int signum);
int	MAK_arr_size(char **arr);

char *get_cwd_path();
char **get_envp_path(char **envp);
char *get_home_path(char **envp);
char *is_relative_path(const char *str); // TODO not needed
int is_executable(const char *str, char **paths); // TODO not needed
char *return_executable_path(const char *str, char **paths);

int try_builtin(t_cmd_table *table);

int builtin_chdir(char **args);
int builtin_pwd(char **args);
int builtin_env(char **args);
int builtin_echo(char **args);
int builtin_echo_option(char *str);

int builtin_export(char **args); // TODO not finished
int builtin_export_old(char **args); // TODO not finished
char **copy_array(char **arr);
int search_array(char **arr, char *search);
int array_free_and_add(char ***arr, char *new_value);
int array_free_and_remove(char ***arr, char *remove_value);
/* int array_free_and_remove_2(char ***arr, char *remove_value); */

int builtin_unset(char **args);
int builtin_exit(char **args);
int is_numeric(const char *str);

void test_execute(char* input, char **paths, char **envp);
int test_fork(char* exec_path, char **input_split, char **envp);

int key_valid(char *str); // TODO Rename // TODO Rename
int key_search_remove(char *str); // TODO CRAP

// temp.c DEBUG
void DEBUG_print_strings(char **arr);
void DEBUG_is_executable(char **paths);





// DOMAGOJ
t_token			*cut_token(t_token *token);
t_cmd_table		*parse(const char *input);
t_token			*tokenize(char *input_string);
size_t			token_len(char *str);
void			token_categorisation(t_token **tokens);
int				make_token_entry(t_token **tokens, char *str_token);
t_cmd_table		*table_init(size_t of_size);
int				token_distribution(t_cmd_table *table, t_token *token);
int				arr_create(t_cmd_table *table, t_token *token);
t_err			expander(t_token *tokens);
int				exit_status(char **str);
int				expand_env(char **arg);
int				is_whitespace(char c);
int				skip_whitespace(char **str);
void			free_table(t_cmd_table *table);
void			cmd_print(t_cmd_table *table);
void			add_data_previous(t_redir_data *lst);
char			*ft_strexpel(char *str, const char *expel, t_err mode);
int				*find_indexes(char *haystack, char *needle);
int				ft_str_insert(char **src, char *insert, size_t place);
int				ft_trim_quotes(char **str);
void			cmd_print(t_cmd_table *table);
t_redir_data	*reverse_data_lst(t_redir_data *data);
t_token			*reverse_lst(t_token *lst);
void			free_lst(t_token *lst);
void			add_previous(t_token *lst);
int				input_check(const char *input);
size_t			table_size(t_token *tokens);
t_redir_data	*add_redir_entry(t_redir_data *data, t_token *token);
char			*getenv_local(char *name);
t_err			execute(t_cmd_table *table);
int				pipe_setup(t_cmd_table *table);
int				close_pipes(t_cmd_table *table);
int				heredoc(t_cmd_table *table);
int				ft_create_file(char **filename);
int				ft_append(const char *str, int fd);
char			**get_path(const char *cmd);
void			ft_free_array(char **arr);
void			syntax_error_print(char *reason);
int				connect_pipes(t_cmd_table *table);
t_err			redir(t_cmd_table *table);
void			finish_redir_data_lst(t_cmd_table *table);
char			*fetch_test(int counter);
#endif
