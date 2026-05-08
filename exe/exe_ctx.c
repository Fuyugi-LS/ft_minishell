/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_ctx.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "exe_struct.h"
#include "ft_fprintf.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

/**
 * shell_get_env - Find the value of an environment variable
 * @envp: Environment array
 * @key: Variable name
 *
 * Return: Pointer to value string, or NULL
 */
char	*shell_get_env(char **envp, char *key)
{
	int	i;
	int	len;

	if (!key || !envp)
		return (NULL);
	len = ft_strlen(key);
	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
	}
	return (NULL);
}

static int	open_pipes(t_exec_context *context)
{
	int	i;

	context->pipes = malloc(sizeof (int) * 2 * (context->count - 1));
	if (!context->pipes)
	{
		ft_fprintf(2, "minishell: malloc failed\n", NULL);
		free(context->pids);
		context->pids = NULL;
		return (1);
	}
	i = -1;
	while (++i < context->count - 1)
	{
		if (pipe(context->pipes[i]) == -1)
		{
			ft_fprintf(2, "minishell: pipe failed\n", NULL);
			free(context->pids);
			context->pids = NULL;
			free(context->pipes);
			context->pipes = NULL;
			return (1);
		}
	}
	return (0);
}

int	exe_context_init(t_exec_context *context)
{
	context->pids = malloc(sizeof (pid_t) * context->count);
	if (!context->pids)
	{
		ft_fprintf(2, "minishell: malloc failed\n", NULL);
		return (1);
	}
	context->pipes = NULL;
	if (context->count > 1)
		return (open_pipes(context));
	return (0);
}

/**
 * exe_get_path - Find the PATH value from envp
 * @envp: Environment array
 *
 * Return: Pointer to PATH value string, or NULL
 */
char	*exe_get_path(char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
	{
		if (envp[i][0] == 'P' && envp[i][1] == 'A'
			&& envp[i][2] == 'T' && envp[i][3] == 'H'
			&& envp[i][4] == '=')
			return (envp[i] + 5);
	}
	return (NULL);
}

/**
 * exe_free_split - Free a null-terminated string array
 * @split: The array to free
 */
void	exe_free_split(char **split)
{
	int	i;

	i = -1;
	while (split[++i])
	{
		free(split[i]);
		split[i] = NULL;
	}
	free(split);
}
