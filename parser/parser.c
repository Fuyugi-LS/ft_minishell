/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_types.h"
#include "parser.h"
#include "parser_utils.h"
#include "expander.h"
#include "arena.h"
#include "libft.h"
#include "ft_fprintf.h"

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


static void	add_redir_node(t_shell *shell, t_redir **list, t_redir_type type, char *file)
{
	t_redir	*new;
	t_redir	*last;

	new = arena_alloc(&shell->arena, sizeof(t_redir));
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

int	parse_redirection(t_shell *shell, t_redir **redirs, t_token **t)
{
	char	*val;
	void	*a[1];
	t_token	*cur;

	cur = *t;
	if (!cur->next || cur->next->type != TOK_WORD)
	{
		a[0] = (cur->next ? cur->next->value : "newline");
		ft_fprintf(2, "minishell: syntax error near unexpected token `%s'\n", a);
		shell->last_exit = 2;
		shell->error_printed = 1;
		return (1);
	}
	val = cur->next->value;
	val = finalize_word(val);
	if (cur->type == TOK_REDIR_IN)
		add_redir_node(shell, redirs, REDIR_IN, val);
	else if (cur->type == TOK_REDIR_OUT)
		add_redir_node(shell, redirs, REDIR_OUT, val);
	else if (cur->type == TOK_REDIR_APPEND)
		add_redir_node(shell, redirs, REDIR_APPEND, val);
	else if (cur->type == TOK_REDIR_HEREDOC)
		add_redir_node(shell, redirs, REDIR_HEREDOC, val);
	*t = cur->next->next;
	return (0);
}

static int	fill_cmd(t_shell *shell, t_cmd *cmd, t_token **tok)
{
	int		wi;
	t_token	*t;

	t = *tok;
	wi = 0;
	cmd->args = arena_alloc(&shell->arena, sizeof(char *) * 1024);
	if (!cmd->args)
		return (1);
	ft_memset(cmd->args, 0, sizeof(char *) * 1024);
	while (t && t->type != TOK_PIPE && t->type != TOK_AND && t->type != TOK_OR 
		&& t->type != TOK_LPAREN && t->type != TOK_RPAREN && t->type != TOK_SEMICOLON)
	{
		if (t->type == TOK_WORD)
			cmd->args[wi++] = t->value;
		else if (t->type == TOK_REDIR_IN || t->type == TOK_REDIR_OUT 
			|| t->type == TOK_REDIR_APPEND || t->type == TOK_REDIR_HEREDOC)
		{
			if (parse_redirection(shell, &cmd->redirs, &t))
				return (1);
			continue ;
		}
		else if (t->type == TOK_ERROR)
		{
			shell->last_exit = 2;
			return (1);
		}
		t = t->next;
	}
	cmd->args[wi] = NULL;
	*tok = t;
	return (0);
}

/**
 * parse_tokens - Build a single t_cmd from token list
 * @shell: Shell context
 * @tokens: Token list from lexer
 * @cmd_count: Output: 1 on success
 *
 * Return: Flat t_cmd array (size 1) or NULL
 */
t_cmd	*parse_tokens(t_shell *shell, t_token **tokens, int *cmd_count)
{
	t_cmd	*table;
	t_token	*cur;

	if (!*tokens || !tokens)
	{
		*cmd_count = 0;
		return (NULL);
	}
	if ((*tokens)->type == TOK_PIPE || (*tokens)->type == TOK_AND || (*tokens)->type == TOK_OR 
		|| (*tokens)->type == TOK_AMPERSAND || (*tokens)->type == TOK_ERROR || (*tokens)->type == TOK_SEMICOLON)
	{
		shell->last_exit = 2;
		return (NULL);
	}
	*cmd_count = 1;
	table = arena_alloc(&shell->arena, sizeof(t_cmd));
	if (!table)
		return (NULL);
	ft_memset(table, 0, sizeof(t_cmd));
	cur = *tokens;
	if (fill_cmd(shell, table, &cur))
	{
		*tokens = cur;
		return (NULL);
	}
	*tokens = cur;
	return (table);
}
