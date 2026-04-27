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
#include "parser_utils.h"
#include "expander.h"
#include "arena.h"
#include "ft_fprintf.h"

static int	count_cmds(t_token *t)
{
	int	n;

	n = 1;
	while (t)
	{
		if (t->type == TOK_PIPE)
			n++;
		t = t->next;
	}
	return (n);
}

static int	count_args(t_token *t)
{
	int	n;

	n = 0;
	while (t && t->type != TOK_PIPE)
	{
		if (t->type == TOK_WORD)
			n++;
		else
		{
			if (t->next)
				t = t->next;
		}
		t = t->next;
	}
	return (n);
}

static t_token	*handle_redir(t_shell *shell, t_cmd *cmd, t_token *t)
{
	char	*val;

	if (!t->next)
		return (NULL);
	val = expand_word(shell, t->next->value);
	if (t->type == TOK_REDIR_IN)
		cmd->infile = val;
	else if (t->type == TOK_REDIR_OUT)
		cmd->outfile = val;
	else if (t->type == TOK_REDIR_APPEND)
	{
		cmd->outfile = val;
		cmd->append_mode = 1;
	}
	else if (t->type == TOK_REDIR_HEREDOC)
		cmd->heredoc_delim = val;
	if (t->next)
		return (t->next->next);
	return (t->next);
}

static t_token	*fill_cmd(t_shell *shell, t_cmd *cmd, t_token *t)
{
	int	wc;
	int	wi;

	wc = count_args(t);
	cmd->args = arena_alloc(&shell->arena, sizeof(char *) * (wc + 1));
	if (!cmd->args)
		return (NULL);
	wi = 0;
	while (t && t->type != TOK_PIPE)
	{
		if (t->type == TOK_WORD)
			cmd->args[wi++] = expand_word(shell, t->value);
		else
		{
			t = handle_redir(shell, cmd, t);
			continue ;
		}
		t = t->next;
	}
	cmd->args[wi] = NULL;
	if (t && t->type == TOK_PIPE)
		t = t->next;
	return (t);
}

/**
 * parse_tokens - Build the flat command table from token list
 * @shell: Shell context
 * @tokens: Token list from lexer
 * @cmd_count: Output: number of commands
 *
 * Return: Flat t_cmd array or NULL
 */
t_cmd	*parse_tokens(t_shell *shell, t_token *tokens, int *cmd_count)
{
	t_cmd	*table;
	t_token	*cur;
	int		i;

	if (!tokens)
	{
		*cmd_count = 0;
		return (NULL);
	}
	*cmd_count = count_cmds(tokens);
	table = cmd_table_new(&shell->arena, *cmd_count);
	if (!table)
		return (NULL);
	cur = tokens;
	i = -1;
	while (++i < *cmd_count)
		cur = fill_cmd(shell, &table[i], cur);
	return (table);
}
