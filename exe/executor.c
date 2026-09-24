/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ms_signal.h"
#include "ms_parser.h"
#include "ms_exec.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

static void	execute_subshell(t_shell_data *shell, t_ast_node *node)
{
	pid_t	pid;
	int		status;

	signals_ignore();
	pid = fork();
	if (pid == 0)
	{
		signals_child_reset();
		close_extra_fds();
		if (setup_redirs(shell, node->redirs))
		{
			child_cleanup(shell);
			exit(1);
		}
		execute_ast(shell, node->left);
		child_cleanup(shell);
		exit(shell->last_exit);
	}
	waitpid(pid, &status, 0);
	init_signals();
	apply_wait_status(shell, status);
}

static void	execute_pipe_node(t_shell_data *shell, t_ast_node *node)
{
	int		p[2];
	pid_t	pid;
	pid_t	pid2;
	int		status;

	if (pipe(p) < 0)
		return ;
	signals_ignore();
	pid = fork();
	if (pid == 0)
		run_pipe_child(shell, node, p, 0);
	pid2 = fork();
	if (pid2 == 0)
		run_pipe_child(shell, node, p, 1);
	close(p[0]);
	close(p[1]);
	waitpid(pid, &status, 0);
	waitpid(pid2, &status, 0);
	init_signals();
	apply_wait_status(shell, status);
}

void	execute_commands(t_shell_data *shell, t_command *cmds, int count)
{
	t_exec_context	context;
	int				status;
	int				i;

	if (!cmds || count <= 0 || exec_builtin_single(shell, cmds, count))
		return ;
	if (exe_context_init(&context, cmds, count, shell))
		return ;
	signals_ignore();
	i = -1;
	while (++i < count)
		run_child(&context, i);
	close_all_pipes(context.pipes, count - 1);
	i = -1;
	while (++i < count)
		waitpid(context.pids[i], &status, 0);
	init_signals();
	apply_wait_status(shell, status);
	update_underscore(shell, cmds[count - 1].args);
	exe_context_free(&context);
}

void	execute_ast(t_shell_data *shell, t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == NODE_COMMAND)
		execute_commands(shell, node->cmds, node->count);
	else if (node->type == NODE_AND || node->type == NODE_OR
		|| node->type == NODE_SEQ)
	{
		execute_ast(shell, node->left);
		if (node->type == NODE_SEQ
			|| (node->type == NODE_AND && shell->last_exit == 0)
			|| (node->type == NODE_OR && shell->last_exit != 0))
			execute_ast(shell, node->right);
	}
	else if (node->type == NODE_SUBSHELL)
		execute_subshell(shell, node);
	else if (node->type == NODE_PIPE)
		execute_pipe_node(shell, node);
}
