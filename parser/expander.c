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
#include "expander.h"
#include "exe_ctx_utils.h"
#include "libft.h"
#include "arena.h"

static void	append_str(t_mem_arena **a, char **res, char *add, int tag)
{
	char	*tmp;
	int		i;

	if (!add)
		return ;
	tmp = ft_strdup(add);
	if (tag)
	{
		i = -1;
		while (tmp[++i])
			if (tmp[i] == '*')
				tmp[i] = 1;
	}
	if (!*res)
	{
		*res = arena_alloc(a, ft_strlen(tmp) + 1);
		ft_memcpy(*res, tmp, ft_strlen(tmp) + 1);
	}
	else
	{
		i = ft_strlen(*res) + ft_strlen(tmp) + 1;
		*res = ft_memcpy(arena_alloc(a, i), *res, ft_strlen(*res) + 1);
		ft_memcpy(*res + ft_strlen(*res), tmp, ft_strlen(tmp) + 1);
	}
	free(tmp);
}

static void	get_var_name(char *s, int *i, char *name_out)
{
	int	start;
	int	j;

	start = *i;
	if (s[*i] == '?')
		(*i)++;
	else
		while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
			(*i)++;
	j = *i - start;
	ft_memcpy(name_out, s + start, j);
	name_out[j] = '\0';
}

static void	handle_expansion(t_shell_data *shell, char *s, int *i,
				t_word_expand *h)
{
	char	name[256];
	char	*val;

	(*i)++;
	if (!s[*i] || (!ft_isalnum(s[*i]) && s[*i] != '_' && s[*i] != '?'))
	{
		append_str(&shell->arena, h->res, "$", 0);
		return ;
	}
	get_var_name(s, i, name);
	if (name[0] == '?' && !name[1])
		val = ft_itoa(shell->last_exit);
	else
		val = shell_get_env(shell->envp, name);
	if (val)
		append_str(&shell->arena, h->res, val, (h->q == 0));
	if (name[0] == '?' && !name[1])
		free(val);
}

static void	process_char(t_shell_data *shell, char *s, int *i, t_word_expand *h)
{
	char	buf[2];

	buf[1] = '\0';
	if (!h->q && (s[*i] == '\'' || s[*i] == '"'))
	{
		h->q = s[(*i)++];
		*(h->seen_q) = 1;
	}
	else if (h->q && s[*i] == h->q)
	{
		h->q = 0;
		(*i)++;
	}
	else if (s[*i] == '$' && h->q != '\'')
		handle_expansion(shell, s, i, h);
	else
	{
		if (s[*i] == '*' && !h->q)
			buf[0] = 1;
		else
			buf[0] = s[*i];
		append_str(&shell->arena, h->res, buf, 0);
		(*i)++;
	}
}

char	*expand_word(t_shell_data *shell, char *s, int *seen_quotes)
{
	char			*res;
	t_word_expand	h;
	int				i;
	int				sq;

	res = NULL;
	h.res = &res;
	h.q = 0;
	sq = 0;
	h.seen_q = &sq;
	i = 0;
	while (s[i])
		process_char(shell, s, &i, &h);
	if (seen_quotes)
		*seen_quotes = sq;
	if (res && res[0] == '\0' && !sq)
		return (NULL);
	if (!res && sq)
	{
		res = arena_alloc(&shell->arena, 1);
		if (res)
			res[0] = '\0';
	}
	return (res);
}
