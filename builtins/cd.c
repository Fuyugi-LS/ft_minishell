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

#include "minishell.h"
#include "ms_env.h"
#include "ms_builtin.h"
#include "libft.h"
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>

static void	prepend_pwd(t_shell_data *shell, const char *target, char *buf)
{
	char	*pwd;
	int		plen;

	pwd = shell_get_env(shell->envp, "PWD");
	if (!pwd || !pwd[0])
	{
		if (!getcwd(buf, PATH_MAX))
			ft_memcpy(buf, ".", 2);
	}
	else
		ft_memcpy(buf, pwd, ft_strlen(pwd) + 1);
	plen = (int)ft_strlen(buf);
	if (plen > 0 && buf[plen - 1] != '/')
	{
		buf[plen] = '/';
		buf[plen + 1] = '\0';
	}
	ft_memcpy(buf + ft_strlen(buf), target, ft_strlen(target) + 1);
}

static void	compute_logical(t_shell_data *shell, const char *target,
		char *buf, char *logical)
{
	const char	*segs[PATH_MAX];
	int			lens[PATH_MAX];

	if (target[0] == '/')
		ft_memcpy(buf, target, ft_strlen(target) + 1);
	else
		prepend_pwd(shell, target, buf);
	parse_segments(buf, segs, lens);
	build_output(segs, lens, buf[0] == '/', logical);
}

static int	cd_getcwd_error(void)
{
	ft_putstr_fd("minishell: cd: error retrieving current directory: ", 2);
	ft_putendl_fd("getcwd: cannot access parent directories: "
		"No such file or directory", 2);
	return (1);
}

static char	*resolve_target(t_shell_data *shell, char **args, int *pp)
{
	char	*t;

	if (!args[1])
	{
		*pp = 0;
		t = shell_get_env(shell->envp, "HOME");
		if (!t)
			ft_putendl_fd("minishell: cd: HOME not set", 2);
		return (t);
	}
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		*pp = 1;
		t = shell_get_env(shell->envp, "OLDPWD");
		if (!t)
			ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
		return (t);
	}
	*pp = 0;
	if (args[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		return (NULL);
	}
	return (args[1]);
}

int	builtin_cd(t_shell_data *shell, char **args)
{
	char	buf[PATH_MAX];
	char	logical[PATH_MAX];
	char	*target;
	int		pp;

	target = resolve_target(shell, args, &pp);
	if (!target)
		return (1);
	compute_logical(shell, target, buf, logical);
	if (chdir(logical) != 0 && chdir(target) != 0)
	{
		print_error("minishell: cd: ", target,
			": No such file or directory");
		return (1);
	}
	if (!getcwd(buf, PATH_MAX))
		return (cd_getcwd_error());
	update_pwd(shell, logical);
	if (pp)
		ft_putendl_fd(logical, 1);
	return (0);
}
