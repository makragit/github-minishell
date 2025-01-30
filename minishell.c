#include "minishell.h"

volatile sig_atomic_t	g_foreground = 0;

void	sigint_handler_non_interactive(int signum)
{
	rl_on_new_line();
	(void)signum;
	return ;
}

void	sigint_handler(int signum)
{
	if (g_foreground == 0)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	(void)signum;
	return ;
}

int	set_signals(int flag)
{
	struct sigaction	sig_action;

	ft_memset(&sig_action, 0, sizeof(sig_action));
	sigemptyset(&sig_action.sa_mask);
	if (flag == 0)
	{
		sig_action.sa_handler = sigint_handler;
		sig_action.sa_flags = 0;
		sigaction(SIGINT, &sig_action, NULL);
		sig_action.sa_handler = SIG_IGN;
		sigaction(SIGQUIT, &sig_action, NULL);
	}
	else if (flag == 1)
	{
		sig_action.sa_handler = sigint_handler_non_interactive;
		sig_action.sa_flags = 0;
		sigaction(SIGINT, &sig_action, NULL);
		sigaction(SIGQUIT, &sig_action, NULL);
	}
	(void)flag;
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;
	int		ret;
	int		non_interactive_mode;

	if (!envp)
		return (0);
	non_interactive_mode = check_argv(argc, argv);
	data = init_data(envp);
	get_data(data);
	/* funcheck_tests(data); // TODO DEBUG funcheck */
	set_signals(non_interactive_mode);
	if (non_interactive_mode == 1)
		ret = run_non_interactive(argv);
	else
		ret = run_interactive(data);
	free_all();
	return (ret);
}

// runs minishell in interactive, returns last exit code
int	run_interactive(t_data *data)
{
	t_cmd_table	*table;

	while (data->exit_called == 0)
	{
		free(data->input);
		data->input = display_prompt();
		if (data->input == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (*(data->input) == '\0')
			continue ;
		table = parse(data->input);
		get_table_reset(table, 0);
		add_history(data->input);
		if (table)
		{
			g_foreground = 1; // TODO put somewhere in execution?
			execute(table);
			g_foreground = 0; // TODO put somewhere in execution?
			free_table(get_table_reset(NULL, 0));
		}
		get_table_reset(NULL, 1);
	}
	return (get_data(NULL)->last_ex_code);
}

// runs minishell in non-interactive, returns last exit code
int	run_non_interactive(char **argv)
{
	t_cmd_table	*table;

	table = parse(argv[2]);
	if (!table)
		malloc_error("ERROR: get_table");
	get_table_reset(table, 0);
	g_foreground = 1; // TODO put somewhere in execution?
	execute(table);
	return (get_data(NULL)->last_ex_code);
}

// returns 0 on interactive, returns 1 on non-interactive, exits on syntax-error
int	check_argv(int argc, char **av)
{
	if (argc == 1)
		return (0);
	if (check_cmd_option(av[1], 'c') == 1)
	{
		if (argc == 2)
			exit(bsh_err("minishell", av[1], "option requires an argument", 2));
	}
	else if (check_cmd_option(av[1], 'c') == -1)
		exit(bsh_err("minishell", av[1], "invalid option", 2));
	else
		exit(bsh_err("minishell", av[1], "command not found", 127));
	return (1);
}
