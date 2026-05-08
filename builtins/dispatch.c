/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/05/08 06:14:41 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"

/**
 * is_builtin - Check if command name is a builtin
 * @name: Command name string
 *
 * Return: 1 if builtin, 0 otherwise
 */
int	is_builtin(char *name)
{
	if (!name)
		return (0);
	return (ft_strncmp(name, "echo", 5) == 0
		|| ft_strncmp(name, "cd", 3) == 0
		|| ft_strncmp(name, "pwd", 4) == 0
		|| ft_strncmp(name, "env", 4) == 0
		|| ft_strncmp(name, "export", 7) == 0
		|| ft_strncmp(name, "unset", 6) == 0
		|| ft_strncmp(name, "exit", 5) == 0);
}

/**
 * run_builtin - Dispatch to the correct builtin
 * @shell: Shell context
 * @args: Command argument array
 *
 * Return: exit status of the builtin
 */
int	run_builtin(t_shell_data *shell, char **args)
{
	if (ft_strncmp(args[0], "echo", 5) == 0)
		return (builtin_echo(args));
	if (ft_strncmp(args[0], "pwd", 4) == 0)
		return (builtin_pwd(shell));
	if (ft_strncmp(args[0], "cd", 3) == 0)
		return (builtin_cd(shell, args));
	if (ft_strncmp(args[0], "env", 4) == 0)
		return (builtin_env(shell));
	if (ft_strncmp(args[0], "export", 7) == 0)
		return (builtin_export(shell, args));
	if (ft_strncmp(args[0], "unset", 6) == 0)
		return (builtin_unset(shell, args));
	if (ft_strncmp(args[0], "exit", 5) == 0)
	{
		builtin_exit(shell, args);
		return (shell->last_exit);
	}
	return (0);
}
