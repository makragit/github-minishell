/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:31:12 by mkrausho          #+#    #+#             */
/*   Updated: 2025/02/14 17:53:45 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "42-Get-next-line/get_next_line.h" // TODO DEBUG

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
# include <fcntl.h>
# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

# define PROMPT "\001\033[1;36m\002MINI:$ \001\033[0m\002"
# define PROMPT_ROOT "\001\033[1;36m\002MINI:# \001\033[0m\002"

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
	CLOSE,
	RESTORE,
	FILE_,
	DIR,
	BUILTIN,
	NOT_FOUND,
	PERMISSION,
	PERRORR,
	MISSING,
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
	int				is_quoted;
	struct t_token	*next;
	struct t_token	*prev;
}	t_token;

typedef struct t_redir_data
{
	char					*input;
	char					*output;
	char					*append;
	char					*hdoc_delim;
	char					*heredoc_file_name;
	int						is_quoted;
	struct t_redir_data		*next;
	struct t_redir_data		*prev;
}	t_redir_data;

typedef struct t_cmd_table
{
	int					index;
	pid_t				pid;
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
	int		backup_in;
	int		backup_out;
	char	*prompt_str;
	char	**env_paths;
	char	*home_path;
	char	**original_envp;
	char	**mini_envp;
	int		exit_called;
	char	*input;
	char	*last_cwd;
	int		heredoc_active;
}	t_data;

// TODO DEL TESTER
int				run_interactive_test(t_data *data);
char			*get_next_line(int fd);

// new.c
int				cwd_deleted(void);
long			mak_atol(const char *str);

// minishell.c
int				run_non_interactive(char **argv);
int				run_interactive(t_data *data);
int				check_argv(int argc, char **av);

// struct.c
t_data			*get_data(t_data *data);
t_cmd_table		*get_table_reset(t_cmd_table *table, int reset);
t_data			*init_data(char **envp);
char			**create_empty_envp(void);

// error.c
void			exit_error(char *s);
void			malloc_error(char *s);
int				bsh_err(char *cmd, char *arg, char *msg, int code);
char			*bsh_err_help(char *cmd, char *arg, int size, int qte);

// free.c
void			free_data(void);
void			free_all(void);
int				mak_free_array(char **arr);

// utils_array.c
int				mak_arr_size(char **arr);
char			**copy_array(char **arr);
void			mak_arr_sort(char **arr, int size);

// utils_array_2.c
int				array_free_and_add(char ***arr, char *new_value);
int				array_free_and_rem(char ***arr, char *rem_value);
char			**array_rem_help(char **arr, char **new_arr, char *rem);
int				array_add_help(char **arr, char **new_arr, char *new_v);

// prompt
char			*display_prompt(void);
char			*display_prompt_small(void);
char			*prepare_prompt_string(char *user, char *prompt_path, int size);
int				is_root(void);

// signals.c
int				set_signals(int flag);
void			sigint_handler(int signum);
void			sigint_handler_non_interactive(int signum);
int				set_signals_default(void);

// builtin_export.c
int				builtin_export(char **args);
int				export_handle_key_value(char **args, int *i);
void			builtin_export_print(char *arr[], int size);
int				export_update_no_equal(char *arg);
int				export_update_with_value(char *arg);

// builtin_unset.c
int				builtin_unset(char **args);

// key_handling.c
int				search_key_in_array(char **arr, char *search);
int				env_key_valid(char *str);
int				env_key_valid_helper(char c);
void			update_env_var(char *key, char *value);
void			update_env_var_help(char *key, char *key_value);

// builtin_cd.c
int				builtin_chdir(char **args);
void			builtin_chdir_update_pwd(void);
char			*check_tilde(char *str);

// misc.c
int				is_numeric(const char *str);
int				check_cmd_option(char *str, char option);
char			*get_cwd_path(void);
int				len_to_equal_sign(char *str);
int				ft_isspace(char c);

// builtin_general.c
int				try_builtin(t_cmd_table *table);
int				builtin_pwd(char **args);
int				builtin_env(char **args);
int				builtin_echo(char **args);
int				builtin_exit(char **args);

// DOMAGOJ
int				is_builtin(char *cmd);
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
int				expand_env(char **arg, int d_q, int c);
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
int				heredoc(t_cmd_table *table, char *filename);
int				ft_create_file(char **filename);
int				ft_append(const char *str, int fd);
char			*get_path(const char *cmd);
void			ft_free_array(char **arr);
int				syntax_error_print(char *reason);
int				execute_single(t_cmd_table *table, t_cmd_table *head);
t_err			redir(t_redir_data *lst, t_redir_data *lst_head);
void			add_tokens_to_table(t_cmd_table *table, t_token *lst);
char			*fetch_test(int counter);
int				close_unused_pipes(t_cmd_table *head);
int				all_pipes_init(t_cmd_table *table);
int				pipe_redir(t_cmd_table *table);
void			print_path_err(t_err reason, const char *cmd);
t_err			is_proper_exe(const char *path, int flag);
int				skip_s_quote(char *str, int *c, int flag);
char			*read_line(void);
void			compress_str(char *str, size_t str_size);
char			*_var_name(char **str, size_t start, size_t len);

#endif
