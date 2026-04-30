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
#include "ft_fprintf.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <readline/readline.h>
#include "expander.h"
#include "parser.h"
#include "parser_utils.h"

static void	close_all_pipes(int (*pipes)[2], int count)
{
	int	i;

	i = -1;
	while (++i < count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}

static int	setup_redirs(t_shell *shell, t_redir *r)
{
	int		fd;
	char	*val;
	int		sq;
	char	**wild_args;
	int		wi;
	void	*a[1];

	while (r)
	{
		val = expand_word(shell, r->file, &sq);
		if (r->type != REDIR_HEREDOC)
		{
			if (!val || (!sq && ft_strchr(val, ' ')))
			{
				a[0] = r->file;
				ft_fprintf(2, "minishell: %s: ambiguous redirect\n", a);
				return (1);
			}
			if (has_wildcard(val))
			{
				wild_args = arena_alloc(&shell->arena, sizeof(char *) * 1024);
				wi = 0;
				expand_wildcard(shell, val, &wild_args, &wi);
				if (wi > 1)
				{
					a[0] = r->file;
					ft_fprintf(2, "minishell: %s: ambiguous redirect\n", a);
					return (1);
				}
				if (wi == 1)
					val = wild_args[0];
			}
		}
		val = finalize_word(val);
		if (r->type == REDIR_IN)
		{
			fd = open(val, O_RDONLY);
			if (fd < 0)
			{
				a[0] = val;
				if (errno == ENOENT)
					ft_fprintf(2, "minishell: %s: No such file or directory\n", a);
				else
					ft_fprintf(2, "minishell: %s: Permission denied\n", a);
				return (1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (r->type == REDIR_HEREDOC)
		{
			fd = open(".heredoc_tmp", O_RDONLY);
			if (fd < 0)
				return (1);
			dup2(fd, STDIN_FILENO);
			close(fd);
			unlink(".heredoc_tmp");
		}
		else if (r->type == REDIR_OUT || r->type == REDIR_APPEND)
		{
			if (r->type == REDIR_APPEND)
				fd = open(val, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				fd = open(val, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				a[0] = val;
				if (errno == ENOENT)
					ft_fprintf(2, "minishell: %s: No such file or directory\n", a);
				else
					ft_fprintf(2, "minishell: %s: Permission denied\n", a);
				return (1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		r = r->next;
	}
	return (0);
}

static void	process_heredoc_list(t_redir *r)
{
	char	*line;
	int		fd;

	while (r)
	{
		if (r->type == REDIR_HEREDOC)
		{
			fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				return ;
			while (1)
			{
				line = readline("");
				if (!line)
				{
					ft_fprintf(2, "minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", (void *[]){r->file});
					break ;
				}
				if (ft_strncmp(line, r->file, ft_strlen(r->file) + 1) == 0)
				{
					free(line);
					break ;
				}
				write(fd, line, ft_strlen(line));
				write(fd, "\n", 1);
				free(line);
			}
			close(fd);
		}
		r = r->next;
	}
}

static void	wire_child_fds(t_exec_ctx *ctx, int i)
{
	if (i > 0 && dup2(ctx->pipes[i - 1][0], STDIN_FILENO) == -1)
		exit(1);
	if (i < ctx->count - 1 && dup2(ctx->pipes[i][1], STDOUT_FILENO) == -1)
		exit(1);
	if (setup_redirs(ctx->shell, ctx->cmds[i].redirs))
		exit(1);
	close_all_pipes(ctx->pipes, ctx->count - 1);
}

static char	**expand_cmd_args(t_shell *shell, char **args)
{
	char	**new_args;
	char	*word;
	int		i;
	int		wi;
	int		sq;

	if (!args)
		return (NULL);
	new_args = arena_alloc(&shell->arena, sizeof(char *) * 1024);
	if (!new_args)
		return (NULL);
	ft_memset(new_args, 0, sizeof(char *) * 1024);
	i = 0;
	wi = 0;
	while (args[i])
	{
		word = expand_word(shell, args[i], &sq);
		if (word)
		{
			if (has_wildcard(word))
				expand_wildcard(shell, word, &new_args, &wi);
			else
				new_args[wi++] = finalize_word(word);
		}
		i++;
	}
	new_args[wi] = NULL;
	return (new_args);
}

static void	run_child(t_exec_ctx *ctx, int i)
{
	pid_t	pid;
	char	**args;

	pid = fork();
	if (pid < 0)
	{
		ft_fprintf(2, "minishell: fork failed\n", NULL);
		return ;
	}
	if (pid == 0)
	{
		wire_child_fds(ctx, i);
		args = expand_cmd_args(ctx->shell, ctx->cmds[i].args);
		if (args && args[0] && is_builtin(args[0]))
			exit(run_builtin(ctx->shell, args));
		ctx->cmds[i].args = args;
		exe_launch(&ctx->cmds[i], ctx->shell->envp);
	}
	ctx->pids[i] = pid;
}

void	execute_commands(t_shell *shell, t_cmd *cmds, int count)
{
	t_exec_ctx	ctx;
	int			status;
	int			i;

	if (!cmds || count <= 0)
		return ;
	if (count == 1 && (!cmds[0].args || !cmds[0].args[0]))
	{
		int saved_in = dup(0);
		int saved_out = dup(1);
		if (setup_redirs(shell, cmds[0].redirs))
			shell->last_exit = 1;
		else
			shell->last_exit = 0;
		dup2(saved_in, 0);
		dup2(saved_out, 1);
		close(saved_in);
		close(saved_out);
		return ;
	}
	if (count == 1)
	{
		char **args = expand_cmd_args(shell, cmds[0].args);
		if (args && args[0] && is_builtin(args[0]))
		{
			int	saved_in = dup(STDIN_FILENO);
			int	saved_out = dup(STDOUT_FILENO);
			if (setup_redirs(shell, cmds[0].redirs))
			{
				shell->last_exit = 1;
				dup2(saved_in, STDIN_FILENO);
				dup2(saved_out, STDOUT_FILENO);
				close(saved_in);
				close(saved_out);
				return ;
			}
			shell->last_exit = run_builtin(shell, args);
			dup2(saved_in, STDIN_FILENO);
			dup2(saved_out, STDOUT_FILENO);
			close(saved_in);
			close(saved_out);
			return ;
		}
	}
	ctx.cmds = cmds;
	ctx.count = count;
	ctx.shell = shell;
	if (exe_ctx_init(&ctx))
		return ;
	i = -1;
	while (++i < count)
		run_child(&ctx, i);
	close_all_pipes(ctx.pipes, count - 1);
	i = -1;
	while (++i < count)
	{
		waitpid(ctx.pids[i], &status, 0);
		if (i == count - 1)
		{
			if (WIFEXITED(status))
				shell->last_exit = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->last_exit = 128 + WTERMSIG(status);
		}
	}
	exe_ctx_free(&ctx);
}

static void	traverse_ast_heredocs(t_node *node)
{
	int	i;

	if (!node)
		return ;
	if (node->type == NODE_COMMAND)
	{
		i = -1;
		while (++i < node->count)
			process_heredoc_list(node->cmds[i].redirs);
	}
	else
	{
		process_heredoc_list(node->redirs);
		traverse_ast_heredocs(node->left);
		traverse_ast_heredocs(node->right);
	}
}

void	execute_ast(t_shell *shell, t_node *node)
{
	static int	depth = 0;
	pid_t		pid;
	int			status;

	if (!node)
		return ;
	if (depth == 0)
		traverse_ast_heredocs(node);
	depth++;
	if (node->type == NODE_COMMAND)
		execute_commands(shell, node->cmds, node->count);
	else if (node->type == NODE_AND)
	{
		execute_ast(shell, node->left);
		if (shell->last_exit == 0)
			execute_ast(shell, node->right);
	}
	else if (node->type == NODE_OR)
	{
		execute_ast(shell, node->left);
		if (shell->last_exit != 0)
			execute_ast(shell, node->right);
	}
	else if (node->type == NODE_SUBSHELL)
	{
		pid = fork();
		if (pid == 0)
		{
			if (setup_redirs(shell, node->redirs))
				exit(1);
			execute_ast(shell, node->left);
			exit(shell->last_exit);
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->last_exit = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->last_exit = 128 + WTERMSIG(status);
	}
	else if (node->type == NODE_PIPE)
	{
		int p[2];
		if (pipe(p) < 0) return;
		pid = fork();
		if (pid == 0)
		{
			close(p[0]);
			dup2(p[1], STDOUT_FILENO);
			close(p[1]);
			execute_ast(shell, node->left);
			exit(shell->last_exit);
		}
		pid_t pid2 = fork();
		if (pid2 == 0)
		{
			close(p[1]);
			dup2(p[0], STDIN_FILENO);
			close(p[0]);
			execute_ast(shell, node->right);
			exit(shell->last_exit);
		}
		close(p[0]);
		close(p[1]);
		waitpid(pid, &status, 0);
		waitpid(pid2, &status, 0);
		if (WIFEXITED(status))
			shell->last_exit = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->last_exit = 128 + WTERMSIG(status);
	}
	depth--;
}
