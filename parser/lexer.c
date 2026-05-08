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
#include "parser_utils.h"
#include "ft_fprintf.h"
#include "libft.h"
#include <string.h>

static t_lex_token	*lex_op_token(t_mem_arena **arena, char *input, int *ip)
{
	int	start;
	int	i;

	start = *ip;
	i = *ip;
	if (ft_strncmp(input + i, ">>", 2) == 0
		|| ft_strncmp(input + i, "<<", 2) == 0
		|| ft_strncmp(input + i, "&&", 2) == 0
		|| ft_strncmp(input + i, "||", 2) == 0
		|| ft_strncmp(input + i, "|&", 2) == 0)
		i += 2;
	else
		i += 1;
	*ip = i;
	return (new_lex_token(arena,
			arena_substr(arena, input, start, i - start),
			get_tok_type(input + start)));
}

static t_lex_token	*lex_word_token(t_mem_arena **arena, char *input, int *ip)
{
	int		start;
	int		i;
	char	q;

	start = *ip;
	i = *ip;
	q = 0;
	while (input[i])
	{
		if (!q && (input[i] == '\'' || input[i] == '"'))
			q = input[i];
		else if (q && input[i] == q)
			q = 0;
		else if (!q && (ft_strchr("<>|&();", input[i])
				|| input[i] == ' '
				|| (input[i] >= 9 && input[i] <= 13)))
			break ;
		i++;
	}
	*ip = i;
	if (q)
		return (new_lex_token(arena, "quote", TOK_ERROR));
	return (new_lex_token(arena,
			arena_substr(arena, input, start, i - start),
			get_tok_type(input + start)));
}

static void	append_tok(t_lex_token **head, t_lex_token **curr, t_lex_token *tok)
{
	if (!*head)
	{
		*head = tok;
		*curr = *head;
	}
	else
	{
		(*curr)->next = tok;
		*curr = (*curr)->next;
	}
}

t_lex_token	*tokenize_input(t_mem_arena **arena, char *input)
{
	t_lex_token	*head;
	t_lex_token	*curr;
	int			i;

	head = NULL;
	curr = NULL;
	i = 0;
	if (!input)
		return (NULL);
	while (input[i])
	{
		while (input[i] && (input[i] == ' '
				|| (input[i] >= 9 && input[i] <= 13)))
			i++;
		if (!input[i])
			break ;
		if (ft_strchr("<>|&();", input[i]))
			append_tok(&head, &curr, lex_op_token(arena, input, &i));
		else
			append_tok(&head, &curr, lex_word_token(arena, input, &i));
	}
	return (head);
}
