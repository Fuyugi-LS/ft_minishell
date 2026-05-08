/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_exec_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_types.h"
#include "shell.h"
#include "builtin_dispatch.h"
#include "builtins.h"
#include "expander.h"
#include "libft.h"
#include "get_next_line.h"
#include "arena.h"
#include <unistd.h>
#include <stdlib.h>

int		setup_redirs(t_shell_data *shell, t_redirect *r);
char	**expand_cmd_args(t_shell_data *shell, char **args);

void	child_cleanup(t_shell_data *shell)
{
	free(shell->last_input);
	free_env(shell);
	arena_destroy(shell->arena);
	gnl_cleanup();
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

void	close_extra_fds(void)
{
	int	i;

	i = 3;
	while (i < 1024)
	{
		close(i);
		i++;
	}
}

void	update_underscore(t_shell_data *shell, char **args)
{
	char	*under;
	int		i;

	if (!args || !args[0])
		return ;
	i = 0;
	while (args[i + 1])
		i++;
	under = ft_strjoin("_=", args[i]);
	if (!under)
		return ;
	update_env(shell, under);
	free(under);
}

static void	exec_builtin_with_redir(t_shell_data *shell, t_command *cmd,
				char **args)
{
	int	sin;
	int	sout;
	int	err;

	sin = dup(STDIN_FILENO);
	sout = dup(STDOUT_FILENO);
	err = setup_redirs(shell, cmd->redirs);
	if (err)
		shell->last_exit = 1;
	else if (args && args[0])
		shell->last_exit = run_builtin(shell, args);
	dup2(sin, STDIN_FILENO);
	dup2(sout, STDOUT_FILENO);
	close(sin);
	close(sout);
}

int	exec_builtin_single(t_shell_data *shell, t_command *cmds, int count)
{
	char	**args;

	if (count != 1)
		return (0);
	args = expand_cmd_args(shell, cmds[0].args);
	if (args && args[0] && !is_builtin(args[0]))
		return (0);
	update_underscore(shell, args);
	if (cmds[0].redirs)
		exec_builtin_with_redir(shell, &cmds[0], args);
	else if (args && args[0])
		shell->last_exit = run_builtin(shell, args);
	else
		shell->last_exit = 0;
	return (1);
}
