/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_types.h"
#include "exe_ctx_utils.h"
#include "ft_fprintf.h"
#include "libft.h"
#include "shell.h"
#include "builtins.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

static char	*search_path_dirs(char *cmd, char **dirs)
{
	size_t	dlen;
	size_t	clen;
	char	*candidate;
	int		i;

	i = -1;
	while (dirs[++i])
	{
		dlen = ft_strlen(dirs[i]);
		clen = ft_strlen(cmd);
		candidate = malloc(dlen + clen + 2);
		if (!candidate)
			continue ;
		ft_memcpy(candidate, dirs[i], dlen);
		candidate[dlen] = '/';
		ft_memcpy(candidate + dlen + 1, cmd, clen + 1);
		if (access(candidate, X_OK) == 0)
		{
			exe_free_split(dirs);
			return (candidate);
		}
		free(candidate);
	}
	exe_free_split(dirs);
	return (NULL);
}

void	exe_context_free(t_exec_context *context)
{
	free(context->pids);
	context->pids = NULL;
	free(context->pipes);
	context->pipes = NULL;
}

static char	*find_path(char *cmd, char **envp)
{
	char	*path_env;
	char	**dirs;

	if (!cmd || !*cmd)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, F_OK) == 0)
			return (cmd);
		return (NULL);
	}
	path_env = exe_get_path(envp);
	if (!path_env)
		return (cmd);
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (cmd);
	return (search_path_dirs(cmd, dirs));
}

static void	handle_cmd_err(t_shell_data *shell, char *name, int mode)
{
	void	*err[1];
	int		code;

	err[0] = name;
	code = 127;
	if (mode == -1)
		ft_fprintf(2, "minishell: %s: Is a directory\n", err);
	else if (mode == 0 && (name[0] == '/' || name[0] == '.'))
		ft_fprintf(2, "minishell: %s: No such file or directory\n", err);
	else if (mode == 0)
		ft_fprintf(2, "minishell: %s: command not found\n", err);
	else if (errno == EACCES)
	{
		ft_fprintf(2, "minishell: %s: Permission denied\n", err);
		code = 126;
	}
	else if (errno == ENOENT)
		ft_fprintf(2, "minishell: %s: No such file or directory\n", err);
	else
		ft_fprintf(2, "minishell: %s: command not found\n", err);
	if (mode == -1)
		code = 126;
	child_cleanup(shell);
	exit(code);
}

void	exe_launch(t_command *cmd, t_shell_data *shell)
{
	char		*path;
	char		**envp;
	struct stat	st;
	char		*env_arg;

	envp = shell->envp;
	if (!cmd->args || !cmd->args[0])
	{
		child_cleanup(shell);
		exit(0);
	}
	path = find_path(cmd->args[0], envp);
	if (!path)
		handle_cmd_err(shell, cmd->args[0], 0);
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		handle_cmd_err(shell, cmd->args[0], -1);
	env_arg = ft_strjoin("_=", path);
	if (env_arg)
	{
		update_env(shell, env_arg);
		free(env_arg);
	}
	execve(path, cmd->args, shell->envp);
	handle_cmd_err(shell, cmd->args[0], 1);
}
