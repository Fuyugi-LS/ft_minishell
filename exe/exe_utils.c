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
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

/**
 * path_join - Join a dir and a command name with a '/'
 * @dir: The directory string
 * @cmd: The command name
 *
 * Return: A malloc'd joined path or NULL
 */
static char	*path_join(char *dir, char *cmd)
{
	size_t	dlen;
	size_t	clen;
	char	*result;

	dlen = ft_strlen(dir);
	clen = ft_strlen(cmd);
	result = malloc(dlen + clen + 2);
	if (!result)
		return (NULL);
	ft_memcpy(result, dir, dlen);
	result[dlen] = '/';
	ft_memcpy(result + dlen + 1, cmd, clen + 1);
	return (result);
}

/**
 * find_path - Search $PATH for the executable
 * @cmd: Command name
 * @envp: Environment
 *
 * Return: Malloc'd path or NULL if not found
 */
static char	*find_path(char *cmd, char **envp)
{
	char	*path_env;
	char	**dirs;
	char	*candidate;
	int		i;

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
	i = -1;
	while (dirs[++i])
	{
		candidate = path_join(dirs[i], cmd);
		if (candidate && access(candidate, X_OK) == 0)
		{
			exe_free_split(dirs);
			return (candidate);
		}
		free(candidate);
	}
	exe_free_split(dirs);
	return (NULL);
}

#include "shell.h"
#include "builtins.h"

void	exe_launch(t_cmd *cmd, t_shell *shell)
{
	char		*path;
	char		**envp = shell->envp;
	void		*err[1];
	struct stat	st;

	if (!cmd->args || !cmd->args[0])
		exit(0);
	path = find_path(cmd->args[0], envp);
	if (!path)
	{
		err[0] = cmd->args[0];
		if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
			ft_fprintf(2, "minishell: %s: No such file or directory\n", err);
		else
			ft_fprintf(2, "minishell: %s: command not found\n", err);
		exit(127);
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		err[0] = cmd->args[0];
		ft_fprintf(2, "minishell: %s: Is a directory\n", err);
		exit(126);
	}
	{
		char *env_arg = ft_strjoin("_=", path);
		if (env_arg)
		{
			update_env(shell, env_arg);
			free(env_arg);
		}
	}
	execve(path, cmd->args, shell->envp);
	err[0] = cmd->args[0];
	if (errno == EACCES)
	{
		ft_fprintf(2, "minishell: %s: Permission denied\n", err);
		exit(126);
	}
	if (errno == ENOENT)
	{
		ft_fprintf(2, "minishell: %s: No such file or directory\n", err);
		exit(127);
	}
	ft_fprintf(2, "minishell: %s: command not found\n", err);
	exit(127);
}
