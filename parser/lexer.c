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

static t_token	*new_token(struct s_arena **a, t_token_type t, char *v)
{
	t_token	*tok;

	tok = arena_alloc(a, sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->type = t;
	tok->value = v;
	tok->next = NULL;
	return (tok);
}

static t_token	*tok_op(struct s_arena **a, char *s, int *i)
{
	if (s[*i] == '|')
	{
		(*i)++;
		return (new_token(a, TOK_PIPE, NULL));
	}
	if (s[*i] == '<' && s[*i + 1] == '<')
	{
		(*i) += 2;
		return (new_token(a, TOK_REDIR_HEREDOC, NULL));
	}
	if (s[*i] == '>' && s[*i + 1] == '>')
	{
		(*i) += 2;
		return (new_token(a, TOK_REDIR_APPEND, NULL));
	}
	if (s[*i] == '<')
	{
		(*i)++;
		return (new_token(a, TOK_REDIR_IN, NULL));
	}
	(*i)++;
	return (new_token(a, TOK_REDIR_OUT, NULL));
}

static char	*extract_word(struct s_arena **a, char *s, int *i)
{
	int		start;
	char	quote;
	char	*dst;
	int		len;

	start = *i;
	quote = 0;
	while (s[*i])
	{
		if (!quote && (s[*i] == '\'' || s[*i] == '"'))
			quote = s[(*i)++];
		else if (quote && s[*i] == quote)
			quote = s[(*i)++] * 0;
		else if (!quote && (s[*i] == ' ' || s[*i] == '\t'
				|| s[*i] == '|' || s[*i] == '<' || s[*i] == '>'))
			break ;
		else
			(*i)++;
	}
	len = *i - start;
	dst = arena_alloc(a, len + 1);
	if (!dst)
		return (NULL);
	ft_memcpy(dst, s + start, len);
	dst[len] = '\0';
	return (dst);
}

/**
 * tokenize_input - Lex raw input into a token list
 * @arena: Arena allocator
 * @input: Raw readline string
 *
 * Return: Head of token list or NULL
 */
t_token	*tokenize_input(struct s_arena **arena, char *input)
{
	t_token	head;
	t_token	*cur;
	t_token	*tok;
	int		i;

	if (!input)
		return (NULL);
	head.next = NULL;
	cur = &head;
	i = 0;
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (!input[i])
			break ;
		if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			tok = tok_op(arena, input, &i);
		else
			tok = new_token(arena, TOK_WORD, extract_word(arena, input, &i));
		if (!tok)
			return (NULL);
		cur->next = tok;
		cur = tok;
	}
	return (head.next);
}
