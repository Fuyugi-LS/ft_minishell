/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_types.h"
#include "arena.h"
#include "ft_fprintf.h"
#include "libft.h"
#include <string.h>

static char	*arena_substr(t_arena **a, char *s, int start, int len)
{
	char	*res;

	res = arena_alloc(a, len + 1);
	if (!res)
		return (NULL);
	ft_memcpy(res, s + start, len);
	res[len] = '\0';
	return (res);
}

static t_token_type	get_type(char *s)
{
	if (ft_strncmp(s, ">>", 2) == 0)
		return (TOK_REDIR_APPEND);
	if (ft_strncmp(s, "<<", 2) == 0)
		return (TOK_REDIR_HEREDOC);
	if (ft_strncmp(s, "&&", 2) == 0)
		return (TOK_AND);
	if (ft_strncmp(s, "||", 2) == 0)
		return (TOK_OR);
	if (s[0] == '&')
		return (TOK_AMPERSAND);
	if (s[0] == '|')
		return (TOK_PIPE);
	if (s[0] == '<')
		return (TOK_REDIR_IN);
	if (s[0] == '>')
		return (TOK_REDIR_OUT);
	if (s[0] == '(')
		return (TOK_LPAREN);
	if (s[0] == ')')
		return (TOK_RPAREN);
	if (s[0] == ';')
		return (TOK_SEMICOLON);
	return (TOK_WORD);
}

static t_token	*new_token(t_arena **a, char *val, t_token_type type)
{
	t_token	*t;

	t = arena_alloc(a, sizeof(t_token));
	if (!t)
		return (NULL);
	t->value = val;
	t->type = type;
	t->next = NULL;
	return (t);
}

t_token	*tokenize_input(t_arena **arena, char *input)
{
	t_token	*head = NULL;
	t_token	*curr = NULL;
	int		i = 0;
	int		start;
	char	q;

	if (!input)
		return (NULL);
	while (input[i])
	{
		while (input[i] && (input[i] == ' ' || (input[i] >= 9 && input[i] <= 13)))
			i++;
		if (!input[i])
			break ;
		start = i;
		if (ft_strchr("<>|&();", input[i]))
		{
			if (ft_strncmp(input + i, ">>", 2) == 0 || ft_strncmp(input + i, "<<", 2) == 0
				|| ft_strncmp(input + i, "&&", 2) == 0 || ft_strncmp(input + i, "||", 2) == 0)
				i += 2;
			else
				i += 1;
		}
		else
		{
			q = 0;
			while (input[i])
			{
				if (!q && (input[i] == '\'' || input[i] == '"'))
					q = input[i];
				else if (q && input[i] == q)
					q = 0;
				else if (!q && (ft_strchr("<>|&();", input[i]) || input[i] == ' ' || (input[i] >= 9 && input[i] <= 13)))
					break ;
				i++;
			}
			if (q)
				return (new_token(arena, "quote", TOK_ERROR));
		}
		if (!head)
		{
			head = new_token(arena, arena_substr(arena, input, start, i - start), get_type(input + start));
			curr = head;
		}
		else
		{
			curr->next = new_token(arena, arena_substr(arena, input, start, i - start), get_type(input + start));
			curr = curr->next;
		}
	}
	return (head);
}
