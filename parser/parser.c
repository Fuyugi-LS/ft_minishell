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

int	parse_redirection(t_shell_data *shell, t_redirect **redirs, t_lex_token **t)
{
	void		*a[1];
	t_lex_token	*cur;

	cur = *t;
	if (!cur->next || cur->next->type != TOK_WORD)
	{
		a[0] = "newline";
		if (cur->next)
			a[0] = cur->next->value;
		ft_fprintf(2,
			"minishell: syntax error near unexpected token `%s'\n", a);
		shell->last_exit = 2;
		shell->error_printed = 1;
		return (1);
	}
	if (cur->type == TOK_REDIR_IN)
		add_redir_node(shell, redirs, REDIR_IN, cur->next->value);
	else if (cur->type == TOK_REDIR_OUT)
		add_redir_node(shell, redirs, REDIR_OUT, cur->next->value);
	else if (cur->type == TOK_REDIR_APPEND)
		add_redir_node(shell, redirs, REDIR_APPEND, cur->next->value);
	else if (cur->type == TOK_REDIR_HEREDOC)
		add_redir_node(shell, redirs, REDIR_HEREDOC, cur->next->value);
	*t = cur->next->next;
	return (0);
}

static int	fill_tok(t_shell_data *shell, t_command *cmd,
				t_lex_token **tp, int *wi)
{
	t_lex_token	*t;

	t = *tp;
	if (t->type == TOK_WORD)
		cmd->args[(*wi)++] = t->value;
	else if (is_redir_tok(t))
	{
		if (parse_redirection(shell, &cmd->redirs, &t))
			return (-1);
		*tp = t;
		return (1);
	}
	else if (t->type == TOK_ERROR)
	{
		shell->last_exit = 2;
		return (-1);
	}
	return (0);
}

static int	fill_cmd(t_shell_data *shell, t_command *cmd, t_lex_token **tok)
{
	int			wi;
	t_lex_token	*t;
	int			r;

	t = *tok;
	wi = 0;
	cmd->args = arena_alloc(&shell->arena, sizeof (char *) * 1024);
	if (!cmd->args)
		return (1);
	ft_memset(cmd->args, 0, sizeof (char *) * 1024);
	while (t && t->type != TOK_PIPE && t->type != TOK_AND
		&& t->type != TOK_OR && t->type != TOK_LPAREN
		&& t->type != TOK_RPAREN && t->type != TOK_SEMICOLON)
	{
		r = fill_tok(shell, cmd, &t, &wi);
		if (r == -1)
			return (1);
		if (r == 0)
			t = t->next;
	}
	cmd->args[wi] = NULL;
	*tok = t;
	return (0);
}

static t_command	*alloc_cmd_table(t_shell_data *shell, t_lex_token **tokens)
{
	t_command	*table;
	t_lex_token	*cur;

	table = arena_alloc(&shell->arena, sizeof (t_command));
	if (!table)
		return (NULL);
	ft_memset(table, 0, sizeof (t_command));
	cur = *tokens;
	if (fill_cmd(shell, table, &cur))
	{
		*tokens = cur;
		return (NULL);
	}
	*tokens = cur;
	return (table);
}

t_command	*parse_tokens(t_shell_data *shell, t_lex_token **tokens,
				int *cmd_count)
{
	*cmd_count = 0;
	if (!tokens || !*tokens)
		return (NULL);
	if ((*tokens)->type == TOK_PIPE || (*tokens)->type == TOK_AND
		|| (*tokens)->type == TOK_OR
		|| (*tokens)->type == TOK_AMPERSAND
		|| (*tokens)->type == TOK_ERROR
		|| (*tokens)->type == TOK_SEMICOLON)
	{
		shell->last_exit = 2;
		return (NULL);
	}
	*cmd_count = 1;
	return (alloc_cmd_table(shell, tokens));
}
