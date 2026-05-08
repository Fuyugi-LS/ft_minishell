/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_types.h"
#include "arena.h"
#include "libft.h"

char	*arena_substr(t_mem_arena **a, char *s, int start, int len)
{
	char	*res;

	res = arena_alloc(a, len + 1);
	if (!res)
		return (NULL);
	ft_memcpy(res, s + start, len);
	res[len] = '\0';
	return (res);
}

t_tok_type	get_tok_type(char *s)
{
	if (ft_strncmp(s, ">>", 2) == 0)
		return (TOK_REDIR_APPEND);
	if (ft_strncmp(s, "<<", 2) == 0)
		return (TOK_REDIR_HEREDOC);
	if (ft_strncmp(s, "&&", 2) == 0)
		return (TOK_AND);
	if (ft_strncmp(s, "||", 2) == 0)
		return (TOK_OR);
	if (ft_strncmp(s, "|&", 2) == 0)
		return (TOK_PIPE);
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

t_lex_token	*new_lex_token(t_mem_arena **a, char *val, t_tok_type type)
{
	t_lex_token	*t;

	t = arena_alloc(a, sizeof (t_lex_token));
	if (!t)
		return (NULL);
	t->value = val;
	t->type = type;
	t->next = NULL;
	return (t);
}
