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

typedef struct s_data
{
	char	*prompt_str;
	char	**env_paths;
	char	*home_path;
	char	**envp;

}	t_data;

// temp.c
t_data *get_data(t_data *data);
t_data *init_data(char **envp);
void free_data();
int	free_array(char **arr);

void exit_error(char *s);
char *display_prompt();
char *prepare_prompt_string(char *user, char *path, char *hostname, int size);

int is_root();
int ft_isspace(char c);
void signal_handler(int signum);
int	arr_size(char **arr);

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

void test_execute(char* input, char **paths, char **envp);
int test_fork(char* exec_path, char **input_split, char **envp);

// temp.c DEBUG
void DEBUG_print_strings(char **arr);
void DEBUG_is_executable(char **paths);

#endif
