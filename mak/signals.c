/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:33:14 by mkrausho          #+#    #+#             */
/*   Updated: 2025/02/01 19:33:14 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	sigint_handler_non_interactive(int signum)
{
	rl_on_new_line();
	(void)signum;
	return ;
}
