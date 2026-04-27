/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "exe_ctx_utils.h"
#include "libft.h"
#include "arena.h"

static char	*get_var_name(char *s, int *i)
{
	int		start;
	char	*name;

	start = *i;
	if (s[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	name = ft_substr(s, start, *i - start);
	return (name);
}

static void	append_str(t_arena **a, char **res, char *add)
{
	char	*tmp;

	if (!add)
		return ;
	if (!*res)
	{
		*res = arena_alloc(a, ft_strlen(add) + 1);
		ft_memcpy(*res, add, ft_strlen(add) + 1);
		return ;
	}
	tmp = arena_alloc(a, ft_strlen(*res) + ft_strlen(add) + 1);
	ft_memcpy(tmp, *res, ft_strlen(*res));
	ft_memcpy(tmp + ft_strlen(*res), add, ft_strlen(add) + 1);
	*res = tmp;
}

static void	handle_expansion(t_shell *shell, char *s, int *i, char **res)
{
	char	*name;
	char	*val;

	(*i)++;
	name = get_var_name(s, i);
	if (ft_strncmp(name, "?", 2) == 0)
		val = ft_itoa(shell->last_exit);
	else
		val = shell_get_env(shell->envp, name);
	if (val)
		append_str(&shell->arena, res, val);
	if (ft_strncmp(name, "?", 2) == 0)
		free(val);
	free(name);
}

char	*expand_word(t_shell *shell, char *s)
{
	char	*res;
	char	q;
	int		i;
	char	buf[2];

	res = NULL;
	q = 0;
	i = 0;
	buf[1] = '\0';
	while (s[i])
	{
		if (!q && (s[i] == '\'' || s[i] == '"'))
			q = s[i++];
		else if (q && s[i] == q)
			q = (s[i++] * 0);
		else if (s[i] == '$' && q != '\'')
			handle_expansion(shell, s, &i, &res);
		else
		{
			buf[0] = s[i++];
			append_str(&shell->arena, &res, buf);
		}
	}
	if (!res)
	{
		res = arena_alloc(&shell->arena, 1);
		if (res)
			res[0] = '\0';
	}
	return (res);
}
