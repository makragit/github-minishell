/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_program.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 16:44:59 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/05 19:54:02 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../../../minishell.h"



t_err cmp_line(const char *str1, const char *str2)
{
	int return_value;
	if (!str2 || !str1)
		return (ERROR);
	if (str2[0] == '\'' || str2[0] == '\"')
		return_value = ft_strncmp(str1, str2 + 1, ft_strlen(str2 + 1) - 1);
	else
		return_value = ft_strncmp(str1, str2, ft_strlen(str2));
	if (return_value == 0)
		return (EQUAL);
	return (FAIL);
}

char *expand_line(char *line, char *delim)
{
	if (!line || !delim)
		return (NULL);
	if (delim[0] == '\'' || delim[0] == '\"')
		return (NULL);


	expand_exit_status(&line);
	expand_env(&line);
	ft_trim_quotes(&line);
	return (line);
}

int heredoc(t_cmd_table *table) {

    while (table) {
        t_redir_data *redir_current = table->redir_data; // Local pointer for traversal

        while (redir_current) {
            if (redir_current->heredoc_delimiter) {
                if (table->heredoc_fd == -1) {
                    table->heredoc_fd = ft_create_file();
                }

                char *heredoc_delimiter = redir_current->heredoc_delimiter;
                printf("HEREDOC DELIM IS |%s|\n", heredoc_delimiter);

                while (1) {
                    char *line = readline("readline:");
                    if (!line) { // Handle EOF (Ctrl+D)
                        printf("Unexpected end of input\n");
                        return (ERROR);
                    }

                    if (cmp_line(line, heredoc_delimiter) == EQUAL) {
                        free(line); // Free the line before breaking
                        break;
                    }

                    printf("the line |%s|\n", line);

                    char *expanded_line = expand_line(line, heredoc_delimiter);
                    free(line); // Free the original line after expanding

                    if (!expanded_line) {
                        return (ERROR);
                    }

                    ft_append(expanded_line, table->heredoc_fd);
                    free(expanded_line); // Free expanded line after appending
                }
            }
            redir_current = redir_current->next; // Move to the next redirection
        }
        table = table->next; // Move to the next command table node
    }

    return (0);
}



//*how to do heredoc
/*
	heredoc delimit = get heredoc str!

	if (!heredocs)
		return;
	if (heredocs)
		//CREATE A FILE TO WRITE IN

	while (heredoc_delimiters!)
	{
		//GET A LINE FROM READLINE

		//IF LINE == EOF
				>- move to the next delimiter!
				>- create fd for that file and delete the file
				>- free stuff if remains to be freed
				>- store fd as in cmd_table.redirs.heredocs
		//TOKENIZE THE LINE - and expand
		//SAVE THE LINE TO A FILE- while at it, create append


	}
	create fd for that file
	delete file
	-Have impleented fds for such files









*/
/*
heredoc expansion rules

EOF
->var expansion
'EOF'
->nothing
"EOF"
->same as EOF
EOF - always with no quotes

quotes rules:

'EOF'
- everyhing inside input is kept as it is!!!!! EVERYTHING

"EOF" and EOF
- quotes are removes from lines! - normal token rules regardig expansion of quotes inside quotes

*/
