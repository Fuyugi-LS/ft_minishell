/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ms_env.h"
#include "ms_builtin.h"
#include "libft.h"
#include <unistd.h>
#include <limits.h>

/**
 * builtin_pwd - Print working directory
 * @shell: Shell context
 *
 * Return: 0 on success, 1 on failure
 */
int	builtin_pwd(t_shell_data *shell)
{
	char	buf[PATH_MAX];
	char	*env_pwd;

	env_pwd = shell_get_env(shell->envp, "PWD");
	if (env_pwd)
	{
		ft_putendl_fd(env_pwd, 1);
		return (0);
	}
	if (!getcwd(buf, PATH_MAX))
	{
		ft_putendl_fd("minishell: pwd: getcwd failed", 2);
		return (1);
	}
	ft_putendl_fd(buf, 1);
	return (0);
}
