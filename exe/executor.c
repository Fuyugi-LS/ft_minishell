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

#include "cmd_types.h"
#include "shell.h"
#include "exe_struct.h"
#include "exe_ctx_utils.h"
#include "exe_launch_utils.h"
#include "builtin_dispatch.h"
#include "builtins.h"
#include "ft_fprintf.h"
#include "libft.h"
#include "expander.h"
#include "parser.h"
#include "signal_minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

static void	execute_subshell(t_shell_data *shell, t_ast_node *node)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
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
	if (WIFEXITED(status))
		shell->last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->last_exit = 128 + WTERMSIG(status);
}

static void	execute_pipe_node(t_shell_data *shell, t_ast_node *node)
{
	int		p[2];
	pid_t	pid;
	pid_t	pid2;
	int		status;

	if (pipe(p) < 0)
		return ;
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
	if (WIFEXITED(status))
		shell->last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->last_exit = 128 + WTERMSIG(status);
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
	i = -1;
	while (++i < count)
		run_child(&context, i);
	close_all_pipes(context.pipes, count - 1);
	i = -1;
	while (++i < count)
		waitpid(context.pids[i], &status, 0);
	if (WIFEXITED(status))
		shell->last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->last_exit = 128 + WTERMSIG(status);
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

int	traverse_ast_heredocs(t_ast_node *node)
{
	int	i;
	int	res;

	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
	{
		i = -1;
		while (++i < node->count)
		{
			res = process_heredoc_list(node->cmds[i].redirs);
			if (res)
				return (res);
		}
		return (0);
	}
	res = process_heredoc_list(node->redirs);
	if (res)
		return (res);
	res = traverse_ast_heredocs(node->left);
	if (res)
		return (res);
	return (traverse_ast_heredocs(node->right));
}
