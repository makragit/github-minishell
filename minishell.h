//fix
/*
replace my minishell.c with his
return -Werror to makefile

*/









#ifndef MINISHELL_H
# define MINISHELL_H

#include "./debug/debug.h" // TODO DEBUG
#include "libft/libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <linux/limits.h>

#include <sys/stat.h>
#include <sys/wait.h>

#include <signal.h>

# include <string.h>
# include <ctype.h>
# include <stdbool.h>

extern int	last_executed_cmd;
typedef enum t_err
{
	OK,
	ERROR,
	NO,
	S_QUOTE,
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
	PIPE_TOKEN,
}	t_token_type;

typedef struct t_token
{
	char			*value;
	t_token_type		type;
	struct t_token	*next;
	struct t_token	*prev;
}	t_token;

typedef struct t_redir_data
{
	char					*input;
	char					*output;
	char					*append;
	char					*heredoc_delimiter;
	bool					heredoc_quotes;
	int						fd_in;
	int						fd_out;
	struct t_redir_data		*next;
	struct t_redir_data		*prev;
	//int		stdin_backup;
	//int		stdout_backup;
}	t_redir_data;

typedef struct t_cmd_table
{
	int					index;
	t_token				*tokens;
	char				*cmd;
	char				**args;
	t_redir_data		*redir_data;
	bool				pipe_output;
	int					*pipe_fd;
	struct t_cmd_table	*next;
}	t_cmd_table;

typedef struct s_data
{
	char	*prompt_str;
	char	**env_paths;
	char	*home_path;
	char	**original_envp;
	char	**mini_envp;

}	t_data;

// MAK
// temp.c
t_data *get_data(t_data *data);
t_data *init_data(char **envp);
void free_data();
int	MAK_free_array(char **arr);

void exit_error(char *s);
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

int builtin_check(char *input);
int builtin_chdir(char **split);
int builtin_pwd(char **split);
void builtin_env();
int builtin_echo(char *input);
char *builtin_echo_parse_option(char *str); // TODO use parser/lexer instead
char *builtin_export(char *str); // TODO not finished
char **copy_array(char **arr);

void test_execute(char* input, char **paths, char **envp);
int test_fork(char* exec_path, char **input_split, char **envp);

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
int				expander(t_token *tokens);
int				expand_exit_status(char **str);
int				expand_env(char **arg);
int				is_whitespace(char c);
int				skip_whitespace(char **str);
void			free_table(t_cmd_table *table);
void			cmd_print(t_cmd_table *table);
char			*ft_strexpel(char *haystack, const char *needle);
int				*find_indexes(char *haystack, char *needle);
int				ft_str_insert(char **src, char *insert, size_t place);
int				ft_trim_quotes(char **str);
void			cmd_print(t_cmd_table *table);
t_token			*reverse_lst(t_token *lst);
void			free_lst(t_token *lst);
void			add_previous(t_token *lst);
int				input_check(const char *input);
t_redir_data	*add_redir_entry(t_redir_data *data, t_token *token);
char			*getenv_local(char *name);

// Main-test
int main_test(void);

#endif
