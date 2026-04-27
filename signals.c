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

/**
 * handle_sigint - Handle Ctrl+C (SIGINT)
 * @sig: Signal number
 *
 * Prints a newline and resets the readline prompt,
 * perfectly mimicking bash behavior.
 */
static void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * init_signals - Initialize signal handlers for the REPL
 *
 * Configures SIGINT to be caught and SIGQUIT to be ignored.
 * Uses Absolute Guarding for sigaction().
 */
void	init_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	void				*err_args[1];

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
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
