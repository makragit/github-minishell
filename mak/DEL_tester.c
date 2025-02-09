#include "../minishell.h"

int	run_interactive_test(t_data *data)
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
}

