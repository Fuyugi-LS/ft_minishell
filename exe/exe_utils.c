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

	if (cmd[0] == '/' || cmd[0] == '.')
		return (cmd);
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

/**
 * exe_launch - Resolve path and execve, or exit 127 on fail
 * @cmd: The single command to run
 * @envp: Environment
 */
void	exe_launch(t_cmd *cmd, char **envp)
{
	char	*path;
	void	*err[1];

	if (!cmd->args || !cmd->args[0])
		exit(0);
	path = find_path(cmd->args[0], envp);
	if (!path || execve(path, cmd->args, envp) == -1)
	{
		err[0] = cmd->args[0];
		ft_fprintf(2, "minishell: %s: command not found\n", err);
		if (path != cmd->args[0])
			free(path);
		exit(127);
	}
}
