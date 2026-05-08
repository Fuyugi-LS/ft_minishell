/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_misc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "cmd_types.h"
#include "parser.h"
#include "arena.h"
#include "libft.h"

char	*finalize_word(char *s)
{
	int	i;

	if (!s)
		return (NULL);
	i = -1;
	while (s[++i])
	{
		if (s[i] == 1)
			s[i] = '*';
	}
	return (s);
}

int	has_wildcard(char *s)
{
	if (!s)
		return (0);
	while (*s)
	{
		if (*s == 1)
			return (1);
		s++;
	}
	return (0);
}

int	is_redir_tok(t_lex_token *t)
{
	if (!t)
		return (0);
	return (t->type == TOK_REDIR_IN || t->type == TOK_REDIR_OUT
		|| t->type == TOK_REDIR_APPEND || t->type == TOK_REDIR_HEREDOC);
}

void	add_redir_node(t_shell_data *shell, t_redirect **list,
		t_redir_kind type, char *file)
{
	t_redirect	*new;
	t_redirect	*last;

	new = arena_alloc(&shell->arena, sizeof (t_redirect));
	if (!new)
		return ;
	new->type = type;
	new->file = file;
	new->next = NULL;
	if (!*list)
		*list = new;
	else
	{
		last = *list;
		while (last->next)
			last = last->next;
		last->next = new;
	}
}
