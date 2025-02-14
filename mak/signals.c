/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:33:14 by mkrausho          #+#    #+#             */
/*   Updated: 2025/02/14 17:44:20 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	set_signals(int flag)
{
	struct sigaction	sig_action;

	ft_memset(&sig_action, 0, sizeof(sig_action));
	sigemptyset(&sig_action.sa_mask);
	if (flag == 0 || flag == 1)
	{
		sig_action.sa_handler = sigint_handler;
		sig_action.sa_flags = 0;
		sigaction(SIGINT, &sig_action, NULL);
		sig_action.sa_handler = SIG_IGN;
		sigaction(SIGQUIT, &sig_action, NULL);
	}
	return (0);
}

void	sigint_handler(int signum)
{
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	if (!get_data(NULL)->heredoc_active)
		rl_redisplay();
	get_data(NULL)->last_ex_code = 128 + signum;
	(void)signum;
	return ;
}

void	sigint_handler_non_interactive(int signum)
{
	rl_on_new_line();
	(void)signum;
	return ;
}

int	set_signals_default(void)
{
	struct sigaction	sig_action;

	ft_memset(&sig_action, 0, sizeof(sig_action));
	sigemptyset(&sig_action.sa_mask);
	sig_action.sa_handler = SIG_DFL;
	sig_action.sa_flags = 0;
	sigaction(SIGINT, &sig_action, NULL);
	sig_action.sa_handler = SIG_DFL;
	sig_action.sa_flags = 0;
	sigaction(SIGQUIT, &sig_action, NULL);
	return (0);
}
