#include "../minishell.h"

int	run_interactive_test(t_data *data) // TODO DEBUG
{
	t_cmd_table	*table;
	data->last_ex_code = data->last_ex_code + 0;
	char *line;
	line = get_next_line(fileno(stdin));
	char *corrected = ft_strtrim(line, "\n");
	free(line);
	table = parse(corrected);
	get_table_reset(table, 0);
	if (table)
	{
		execute(table);
		free_table(get_table_reset(NULL, 0));
	}
	get_table_reset(NULL, 1);
	free(corrected);
	return (get_data(NULL)->last_ex_code);
} // TODO DEBUG

// NOT WORKING AS FUNCTION
// int	DEL_funcheck()
// {
// 	// TODO FUNCHECK DEBUG
// 	const char *tests[] = {
// 		/* "echo 'Hello, World!'", */
// 		/* "env | echo hi", */
// 		/* "test", */
// 		/* "echo hello", */
// 		/* "echo", */
// 		/* "echo -n", */
// 		/* "echo hello", */
// 		/* "echo -n hello", */
// 		/* "pwd", */
// 		/* "pwd abc", */
// 		/* "env", */
// 		/* "env abc", */
// 		/* "echo hello", */
// 		/* "export VAR=test", */
// 		/* "export VAR", */
// 		/* "export 1VAR", */
// 		/* "unset PATH", */
// 
//     // Basic Commands
//     /* "env > output/env.txt", */
//     /* "pwd > output/pwd.txt", */
//     /* "echo hello > output/echo_hello.txt", */
//     /* "echo \"hello world\" > output/echo_hello_world.txt", */
//     /* "echo $HOME > output/echo_home.txt", */
//     /* "echo $SHELL > output/echo_shell.txt", */
//     "echo \"A B C\" | cat > output/echo_cat.txt", // funcheck issue
//     /* "echo \"test\" > output/testfile.txt", */
// 
//     // Export and Unset Variables
//     /* "export MYVAR=123", */
//     /* "echo $MYVAR > output/export_myvar.txt", */
//     /* "export MYVAR=\"Hello World\"", */
//     /* "echo $MYVAR > output/export_myvar_hello.txt", */
//     /* "unset MYVAR", */
//     /* "echo $MYVAR > output/unset_myvar.txt", */
//     /* "export PATH=\"/new/path:$PATH\"", */
//     /* "echo $PATH > output/export_path.txt", */
// 
//     // Pipes
//     /* "env | grep PATH > output/pipe_env_grep.txt", */
//     /* "echo \"Hello World\" | tr '[:lower:]' '[:upper:]' > output/pipe_uppercase.txt", */
//     /* "pwd | cat > output/pipe_pwd_cat.txt", */
//     /* "echo $HOME | wc -c > output/pipe_home_wc.txt", */
//     /* "export MYVAR=123 | echo $MYVAR > output/pipe_export_echo.txt", */
// 
//     // Redirections
//     /* "echo \"test output\" > output/redir_out.txt", */
//     /* "cat < output/redir_out.txt > output/redir_cat.txt", */
//     /* "echo \"new line\" >> output/redir_out.txt", */
//     /* "export TEST=abc > output/redir_export.txt", */
//     /* "env > output/env_dump.txt && cat output/env_dump.txt > output/env_dump_check.txt", */
// 
//     // Exit Cases (won't save output since they terminate the shell)
//     /* "exit", */
//     /* "exit 0", */
//     /* "exit 1", */
//     /* "exit 255", */
// 
//     // Invalid & Edge Cases
//     /* "echo \"$UNSETVAR\" > output/edge_unsetvar.txt", */
//     /* "export VAR=\"A=B=C\"", */
//     /* "echo $VAR > output/edge_export_special.txt", */
//     /* "unset > output/edge_unset_empty.txt", */
//     /* "export =INVALID > output/edge_invalid_export.txt", */
//     /* "unset HOME; echo $HOME > output/edge_unset_home.txt", */
//     /* "env | grep VAR > output/edge_env_grep.txt", */
// 
// 		NULL // Sentinel value to mark the end
// 	};
// 	(void)tests;
// 	
// 	for (int i = 0; tests[i] != NULL; i++) {
// 		table = parse(tests[i]);
// 		get_table_reset(table, 0);
// 		if (table) {
// 		    execute(table);
// 		    free_table(table);
// 		}
// 		get_table_reset(NULL, 1);
// 	}
// 	return (get_data(NULL)->last_ex_code);
// 	// TODO FUNCHECK DEBUG
// }