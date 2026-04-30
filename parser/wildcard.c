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

/* Using \x01 as internal wildcard marker */
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

static int	ft_strcmp_unsigned(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	*u1;
	unsigned char	*u2;

	u1 = (unsigned char *)s1;
	u2 = (unsigned char *)s2;
	i = 0;
	while (u1[i] && u2[i] && u1[i] == u2[i])
		i++;
	return (u1[i] - u2[i]);
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
			if (ft_strcmp_unsigned(arr[i], arr[j]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

static char	*finalize_pattern(t_arena **a, char *pat)
{
	int		i;
	char	*res;

	res = arena_alloc(a, ft_strlen(pat) + 1);
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

void	expand_wildcard(t_shell *shell, char *pat, char ***args, int *wi)
{
	DIR				*d;
	struct dirent	*dir;
	int				start_wi;
	int				found;

	start_wi = *wi;
	found = 0;
	d = opendir(".");
	if (!d)
	{
		(*args)[(*wi)++] = finalize_pattern(&shell->arena, pat);
		return ;
	}
	while ((dir = readdir(d)))
	{
		if (dir->d_name[0] == '.' && pat[0] != '.')
			continue ;
		if (match(pat, dir->d_name))
		{
			(*args)[(*wi)++] = arena_alloc(&shell->arena, ft_strlen(dir->d_name) + 1);
			ft_memcpy((*args)[*wi - 1], dir->d_name, ft_strlen(dir->d_name) + 1);
			found = 1;
		}
	}
	closedir(d);
	if (!found)
		(*args)[(*wi)++] = finalize_pattern(&shell->arena, pat);
	else
		sort_args(&(*args)[start_wi], *wi - start_wi);
}
