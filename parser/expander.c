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

static void	append_tagged_str(t_arena **a, char **res, char *add, int tagged)
{
	int		i;
	char	*tmp;

	if (!add)
		return ;
	tmp = ft_strdup(add);
	if (tagged)
	{
		i = -1;
		while (tmp[++i])
		{
			if (tmp[i] == '*')
				tmp[i] = 1;
		}
	}
	append_str(a, res, tmp);
	free(tmp);
}

static void	handle_expansion(t_shell *shell, char *s, int *i, char **res, char q)
{
	char	*name;
	char	*val;

	(*i)++;
	if (!s[*i] || (!ft_isalnum(s[*i]) && s[*i] != '_' && s[*i] != '?'))
	{
		append_str(&shell->arena, res, "$");
		return ;
	}
	name = get_var_name(s, i);
	if (ft_strncmp(name, "?", 2) == 0)
		val = ft_itoa(shell->last_exit);
	else
		val = shell_get_env(shell->envp, name);
	if (val)
		append_tagged_str(&shell->arena, res, val, (q == 0));
	if (ft_strncmp(name, "?", 2) == 0)
		free(val);
	free(name);
}

char	*expand_word(t_shell *shell, char *s, int *seen_quotes)
{
	char	*res;
	char	q;
	int		i;
	char	buf[2];

	res = NULL;
	q = 0;
	i = 0;
	if (seen_quotes)
		*seen_quotes = 0;
	buf[1] = '\0';
	while (s[i])
	{
		if (!q && (s[i] == '\'' || s[i] == '"'))
		{
			q = s[i++];
			if (seen_quotes)
				*seen_quotes = 1;
		}
		else if (q && s[i] == q)
			q = 0;
		else if (s[i] == '$' && q != '\'')
		{
			handle_expansion(shell, s, &i, &res, q);
			if (res && res[0] == '\0' && q == 0)
			{
				/* Empty expansion outside quotes might need special handling */
			}
		}
		else if (s[i] == '*' && !q)
		{
			buf[0] = 1;
			append_str(&shell->arena, &res, buf);
			i++;
		}
		else
		{
			buf[0] = s[i++];
			append_str(&shell->arena, &res, buf);
		}
	}
	if (res && res[0] == '\0' && (!seen_quotes || !*seen_quotes))
		return (NULL);
	if (!res && seen_quotes && *seen_quotes)
	{
		res = arena_alloc(&shell->arena, 1);
		if (res)
			res[0] = '\0';
	}
	return (res);
}
