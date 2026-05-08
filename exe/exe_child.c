/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_child.c                                        :+:      :+:    :+:   */
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
#include "arena.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

void	execute_ast(t_shell_data *shell, t_ast_node *node);
int		setup_redirs(t_shell_data *shell, t_redirect *r);
void	close_extra_fds(void);
void	child_cleanup(t_shell_data *shell);

void	close_all_pipes(int (*pipes)[2], int count)
{
	int	i;

	i = -1;
	while (++i < count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}

char	**expand_cmd_args(t_shell_data *shell, char **args)
{
	char	**new_args;
	char	*word;
	int		i;
	int		wi;
	int		sq;

	if (!args)
		return (NULL);
	new_args = arena_alloc(&shell->arena, sizeof (char *) * 1024);
	if (!new_args)
		return (NULL);
	ft_memset(new_args, 0, sizeof (char *) * 1024);
	i = 0;
	wi = 0;
	while (args[i])
	{
		word = expand_word(shell, args[i], &sq);
		if (word && has_wildcard(word))
			expand_wildcard(shell, word, &new_args, &wi);
		else if (word)
			new_args[wi++] = finalize_word(word);
		i++;
	}
	new_args[wi] = NULL;
	return (new_args);
}

void	run_pipe_child(t_shell_data *shell, t_ast_node *node, int *p, int side)
{
	if (side == 0)
	{
		close(p[0]);
		dup2(p[1], STDOUT_FILENO);
		close(p[1]);
	}
	else
	{
		close(p[1]);
		dup2(p[0], STDIN_FILENO);
		close(p[0]);
	}
	close_extra_fds();
	if (side == 0)
		execute_ast(shell, node->left);
	else
		execute_ast(shell, node->right);
	child_cleanup(shell);
	exit(shell->last_exit);
}

static void	exec_in_child_process(t_exec_context *context, int index)
{
	char	**args;
	int		r;

	if (index > 0
		&& dup2(context->pipes[index - 1][0], STDIN_FILENO) == -1)
		exit(1);
	if (index < context->count - 1
		&& dup2(context->pipes[index][1], STDOUT_FILENO) == -1)
		exit(1);
	if (setup_redirs(context->shell, context->cmds[index].redirs))
		exit(1);
	close_all_pipes(context->pipes, context->count - 1);
	close_extra_fds();
	args = expand_cmd_args(context->shell, context->cmds[index].args);
	if (args && args[0] && is_builtin(args[0]))
	{
		r = run_builtin(context->shell, args);
		child_cleanup(context->shell);
		exit(r);
	}
	context->cmds[index].args = args;
	exe_launch(&context->cmds[index], context->shell);
}

void	run_child(t_exec_context *context, int index)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		ft_fprintf(2, "minishell: fork failed\n", NULL);
		return ;
	}
	if (pid == 0)
		exec_in_child_process(context, index);
	context->pids[index] = pid;
}
