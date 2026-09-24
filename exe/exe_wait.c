/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_wait.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/09/24 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ms_exec.h"
#include "libft.h"
#include <sys/wait.h>
#include <signal.h>

void	apply_wait_status(t_shell_data *shell, int status)
{
	int	sig;

	if (WIFEXITED(status))
		shell->last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		shell->last_exit = 128 + sig;
		if (sig == SIGINT)
			ft_putchar_fd('\n', 2);
		else if (sig == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", 2);
	}
}
