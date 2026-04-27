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

#include <readline/readline.h>

static void	process_heredoc(t_cmd *cmd)
{
	char	*line;
	int		fd;

	if (!cmd->heredoc_delim)
		return ;
	fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return ;
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, cmd->heredoc_delim, ft_strlen(cmd->heredoc_delim) + 1) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	cmd->infile = ".heredoc_tmp";
}

static void	handle_heredocs(t_cmd *cmds, int count)
{
	int	i;

	i = -1;
	while (++i < count)
		process_heredoc(&cmds[i]);
}

static void	setup_redirections(t_cmd *cmd)
{
	int	fd;

	if (cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
			exit(1);
		dup2(fd, STDIN_FILENO);
		close(fd);
		if (cmd->heredoc_delim)
			unlink(".heredoc_tmp");
	}
	if (cmd->outfile)
	{
		if (cmd->append_mode)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			exit(1);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

static void	wire_child_fds(t_exec_ctx *ctx, int i)
{
	if (i > 0 && dup2(ctx->pipes[i - 1][0], STDIN_FILENO) == -1)
	{
		ft_fprintf(2, "minishell: dup2 failed\n", NULL);
		exit(1);
	}
	if (i < ctx->count - 1 && dup2(ctx->pipes[i][1], STDOUT_FILENO) == -1)
	{
		ft_fprintf(2, "minishell: dup2 failed\n", NULL);
		exit(1);
	}
	setup_redirections(&ctx->cmds[i]);
	close_all_pipes(ctx->pipes, ctx->count - 1);
}

static void	run_child(t_exec_ctx *ctx, int i)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		ft_fprintf(2, "minishell: fork failed\n", NULL);
		return ;
	}
	if (pid == 0)
	{
		wire_child_fds(ctx, i);
		exe_launch(&ctx->cmds[i], ctx->shell->envp);
	}
	ctx->pids[i] = pid;
}

/**
 * execute_commands - Run pipeline. Builtins run in parent if count == 1.
 * @shell: Shell context
 * @cmds: Flat command table
 * @count: Number of commands
 */
void	execute_commands(t_shell *shell, t_cmd *cmds, int count)
{
	t_exec_ctx	ctx;
	int			status;
	int			i;

	if (!cmds || count <= 0 || !cmds[0].args || !cmds[0].args[0])
		return ;
	if (count == 1 && is_builtin(cmds[0].args[0]))
	{
		int	saved_in = dup(STDIN_FILENO);
		int	saved_out = dup(STDOUT_FILENO);
		process_heredoc(&cmds[0]);
		setup_redirections(&cmds[0]);
		shell->last_exit = run_builtin(shell, cmds[0].args);
		dup2(saved_in, STDIN_FILENO);
		dup2(saved_out, STDOUT_FILENO);
		close(saved_in);
		close(saved_out);
		return ;
	}
	ctx.cmds = cmds;
	ctx.count = count;
	ctx.shell = shell;
	handle_heredocs(ctx.cmds, ctx.count);
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
			shell->last_exit = WEXITSTATUS(status);
	}
	exe_ctx_free(&ctx);
}
