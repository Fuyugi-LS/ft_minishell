/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "builtins.h"
#include "exe_ctx_utils.h"
#include "ft_fprintf.h"
#include "libft.h"
#include <limits.h>
#include <unistd.h>

/**
 * builtin_cd - Change current directory
 * @shell: Shell context
 * @args: args[1] is the target path
 *
 * Return: 0 on success, 1 on failure
 */
int	builtin_cd(t_shell *shell, char **args)
{
	char	cwd[PATH_MAX];
	char	*old_pwd;
	char	*env_arg;
	void	*a[1];

	if (!args[1])
	{
		ft_fprintf(2, "minishell: cd: missing argument\n", NULL);
		return (1);
	}
	if (args[1] && args[2])
	{
		ft_fprintf(2, "minishell: cd: too many arguments\n", NULL);
		return (1);
	}
	old_pwd = shell_get_env(shell->envp, "PWD");
	if (chdir(args[1]) == -1)
	{
		a[0] = args[1];
		ft_fprintf(2, "minishell: cd: %s: No such file or directory\n", a);
		return (1);
	}
	if (old_pwd)
	{
		env_arg = ft_strjoin("OLDPWD=", old_pwd);
		update_env(shell, env_arg);
		free(env_arg);
	}
	if (getcwd(cwd, PATH_MAX))
	{
		env_arg = ft_strjoin("PWD=", cwd);
		update_env(shell, env_arg);
		free(env_arg);
	}
	return (0);
}
