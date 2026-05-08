/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "parser.h"
#include "libft.h"
#include <dirent.h>

#define WILD 1

static int	match(char *pat, char *str)
{
	if (!*pat && !*str)
		return (1);
	if (*pat == WILD)
	{
		while (*pat == WILD)
			pat++;
		if (!*pat)
			return (1);
		while (*str)
		{
			if (match(pat, str))
				return (1);
			str++;
		}
		return (match(pat, str));
	}
	if (*pat && *str && (unsigned char)*pat == (unsigned char)*str)
		return (match(pat + 1, str + 1));
	return (0);
}

static void	sort_args(char **arr, int n)
{
	int		i;
	int		j;
	char	*tmp;

	i = -1;
	while (++i < n)
	{
		j = i;
		while (++j < n)
		{
			if (ft_strncmp(arr[i], arr[j], 4096) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

static char	*finalize_pattern(t_mem_arena **ar, char *pat)
{
	int		i;
	char	*res;

	res = arena_alloc(ar, ft_strlen(pat) + 1);
	if (!res)
		return (NULL);
	i = -1;
	while (pat[++i])
	{
		if (pat[i] == WILD)
			res[i] = '*';
		else
			res[i] = pat[i];
	}
	res[i] = '\0';
	return (res);
}

static int	scan_dir(t_shell_data *shell, char *pat, char ***args, int *wi)
{
	DIR				*d;
	struct dirent	*dir;
	size_t			len;
	int				found;

	found = 0;
	d = opendir(".");
	if (!d)
		return (0);
	dir = readdir(d);
	while (dir)
	{
		if (!(dir->d_name[0] == '.' && pat[0] != '.') && match(pat,
				dir->d_name))
		{
			len = ft_strlen(dir->d_name) + 1;
			(*args)[(*wi)++] = arena_alloc(&shell->arena, len);
			ft_memcpy((*args)[*wi - 1], dir->d_name, len);
			found = 1;
		}
		dir = readdir(d);
	}
	closedir(d);
	return (found);
}

void	expand_wildcard(t_shell_data *shell, char *pat, char ***args, int *wi)
{
	int	start_wi;
	int	found;

	start_wi = *wi;
	found = scan_dir(shell, pat, args, wi);
	if (!found)
		(*args)[(*wi)++] = finalize_pattern(&shell->arena, pat);
	else
		sort_args(&(*args)[start_wi], *wi - start_wi);
}
