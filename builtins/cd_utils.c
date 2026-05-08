/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
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
#include "libft.h"
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

static int	process_seg(const char **segs, int *lens, int n, const char *s)
{
	const char	*e;
	int			plen;

	e = s;
	while (*e && *e != '/')
		e++;
	plen = (int)(e - s);
	if (plen == 2 && s[0] == '.' && s[1] == '.' && n > 0)
		return (n - 1);
	if (plen && (plen != 1 || s[0] != '.'))
	{
		segs[n] = s;
		lens[n] = plen;
		return (n + 1);
	}
	return (n);
}

int	parse_segments(const char *path, const char **segs, int *lens)
{
	const char	*s;
	int			n;

	s = path + (path[0] == '/');
	n = 0;
	while (*s)
	{
		while (*s == '/')
			s++;
		if (*s)
			n = process_seg(segs, lens, n, s);
		while (*s && *s != '/')
			s++;
	}
	segs[n] = NULL;
	return (n);
}

void	build_output(const char **segs, int *lens, int is_abs, char *out)
{
	int	pos;
	int	i;

	pos = 0;
	if (is_abs && pos < PATH_MAX - 1)
		out[pos++] = '/';
	i = -1;
	while (segs[++i] && pos < PATH_MAX - 2)
	{
		if (i > 0 && pos < PATH_MAX - 1)
			out[pos++] = '/';
		if (pos + lens[i] < PATH_MAX)
		{
			ft_memcpy(out + pos, segs[i], lens[i]);
			pos += lens[i];
		}
	}
	if (pos == 0)
		out[pos++] = '/';
	out[pos] = '\0';
}

void	update_pwd(t_shell_data *shell, char *new_pwd)
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
