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
#include <stdlib.h>

/*
** Normalise an absolute path string by resolving "." and ".." components.
** Does NOT touch the filesystem – purely string manipulation (POSIX -L mode).
*/
static void	normalize_path(const char *path, char *out, int out_size)
{
	const char	*segs[PATH_MAX];
	int			lens[PATH_MAX];
	int			n;
	int			abs_path;
	const char	*s;
	const char	*e;
	int			plen;
	int			i;
	int			pos;

	n = 0;
	abs_path = (path[0] == '/');
	s = path + abs_path;
	while (*s)
	{
		while (*s == '/') s++;
		if (!*s)
			break ;
		e = s;
		while (*e && *e != '/')
			e++;
		plen = (int)(e - s);
		if (plen == 1 && s[0] == '.')
			;
		else if (plen == 2 && s[0] == '.' && s[1] == '.')
		{
			if (n > 0)
				n--;
		}
		else
		{
			segs[n] = s;
			lens[n] = plen;
			n++;
		}
		s = e;
	}
	pos = 0;
	if (abs_path && pos < out_size - 1)
		out[pos++] = '/';
	i = -1;
	while (++i < n && pos < out_size - 2)
	{
		if (i > 0 && pos < out_size - 1)
			out[pos++] = '/';
		if (pos + lens[i] < out_size)
		{
			ft_memcpy(out + pos, segs[i], lens[i]);
			pos += lens[i];
		}
	}
	if (pos == 0)
		out[pos++] = '/';
	out[pos] = '\0';
}

/*
** Build the raw (un-normalised) candidate path in buf, then normalise
** it into logical.  Uses PWD env for relative resolution.
*/
static void	compute_logical(t_shell *shell, const char *target,
		char *buf, char *logical)
{
	char	*pwd;
	int		plen;

	if (target[0] == '/')
	{
		ft_memcpy(buf, target, ft_strlen(target) + 1);
	}
	else
	{
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
	normalize_path(buf, logical, PATH_MAX);
}

static void	update_pwd(t_shell *shell, char *new_pwd)
{
	char	*old;
	char	*ev;

	old = shell_get_env(shell->envp, "PWD");
	if (old)
	{
		ev = ft_strjoin("OLDPWD=", old);
		update_env(shell, ev);
		free(ev);
	}
	ev = ft_strjoin("PWD=", new_pwd);
	update_env(shell, ev);
	free(ev);
}

/**
 * builtin_cd - POSIX logical-mode cd
 *
 * Handles:
 *   cd          -> go to $HOME
 *   cd -        -> go to $OLDPWD, print path
 *   cd <path>   -> logical resolution (no getcwd dependency for PWD)
 *
 * When the physical directory has been deleted we fall back to
 * chdir(target) which the kernel handles via dentry, while PWD is
 * still updated to the computed logical string.
 */
int	builtin_cd(t_shell *shell, char **args)
{
	char	buf[PATH_MAX];
	char	logical[PATH_MAX];
	char	*target;
	void	*a[1];
	int		print_path;

	print_path = 0;
	if (!args[1])
	{
		target = shell_get_env(shell->envp, "HOME");
		if (!target)
		{
			ft_fprintf(2, "minishell: cd: HOME not set\n", NULL);
			return (1);
		}
	}
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		target = shell_get_env(shell->envp, "OLDPWD");
		if (!target)
		{
			ft_fprintf(2, "minishell: cd: OLDPWD not set\n", NULL);
			return (1);
		}
		print_path = 1;
	}
	else
	{
		if (args[2])
		{
			ft_fprintf(2, "minishell: cd: too many arguments\n", NULL);
			return (1);
		}
		target = args[1];
	}
	compute_logical(shell, target, buf, logical);
	if (chdir(logical) == 0)
	{
		update_pwd(shell, logical);
		if (print_path)
		{
			a[0] = logical;
			ft_fprintf(1, "%s\n", a);
		}
		return (0);
	}
	/* Logical path failed (e.g. a component was deleted).
	** Try the raw target – the kernel can navigate ".." even from a
	** deleted directory via its dentry cache.  PWD is still set to
	** the logical string so subsequent pwd output stays sane. */
	if (chdir(target) == 0)
	{
		update_pwd(shell, logical);
		if (print_path)
		{
			a[0] = logical;
			ft_fprintf(1, "%s\n", a);
		}
		return (0);
	}
	a[0] = target;
	ft_fprintf(2, "minishell: cd: %s: No such file or directory\n", a);
	return (1);
}
