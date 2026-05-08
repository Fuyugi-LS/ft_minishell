/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>
#include "ft_fprintf.h"

static void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	if (rl_readline_state & RL_STATE_READCMD)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	signals_child_reset(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	signals_heredoc_mode(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
}

void	init_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	void				*err_args[1];

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) < 0)
	{
		err_args[0] = (void *)"sigaction (SIGINT) failed";
		ft_fprintf(2, "minishell: %s\n", err_args);
		exit(1);
	}
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) < 0)
	{
		err_args[0] = (void *)"sigaction (SIGQUIT) failed";
		ft_fprintf(2, "minishell: %s\n", err_args);
		exit(1);
	}
}
