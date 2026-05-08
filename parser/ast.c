/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "parser.h"
#include "arena.h"
#include "ft_fprintf.h"
#include "libft.h"

static t_ast_node	*parse_primary(t_shell_data *shell, t_lex_token **tokens);
static t_ast_node	*parse_pipeline(t_shell_data *shell, t_lex_token **tokens);
static t_ast_node	*parse_logical(t_shell_data *shell, t_lex_token **tokens);

static t_ast_node	*new_node(t_mem_arena **a, t_node_kind type)
{
	t_ast_node	*n;

	n = arena_alloc(a, sizeof (t_ast_node));
	if (!n)
		return (NULL);
	ft_memset(n, 0, sizeof (t_ast_node));
	n->type = type;
	return (n);
}

static t_ast_node	*parse_primary(t_shell_data *shell, t_lex_token **tokens)
{
	t_ast_node	*node;

	if (!tokens || !*tokens)
		return (NULL);
	if ((*tokens)->type == TOK_LPAREN)
	{
		*tokens = (*tokens)->next;
		node = new_node(&shell->arena, NODE_SUBSHELL);
		node->left = parse_ast(shell, tokens);
		if (!*tokens || (*tokens)->type != TOK_RPAREN)
		{
			shell->last_exit = 2;
			return (NULL);
		}
		*tokens = (*tokens)->next;
		while (is_redir_tok(*tokens))
		{
			if (parse_redirection(shell, &node->redirs, tokens))
				return (NULL);
		}
		return (node);
	}
	node = new_node(&shell->arena, NODE_COMMAND);
	node->cmds = parse_tokens(shell, tokens, &node->count);
	return (node);
}

static t_ast_node	*parse_pipeline(t_shell_data *shell, t_lex_token **tokens)
{
	t_ast_node	*node;
	t_ast_node	*tmp;

	node = parse_primary(shell, tokens);
	if (!node || (node->type == NODE_COMMAND && !node->count))
		return (NULL);
	while (*tokens && (*tokens)->type == TOK_PIPE)
	{
		tmp = new_node(&shell->arena, NODE_PIPE);
		tmp->left = node;
		*tokens = (*tokens)->next;
		tmp->right = parse_pipeline(shell, tokens);
		if (!tmp->right)
		{
			shell->last_exit = 2;
			return (NULL);
		}
		node = tmp;
	}
	return (node);
}

static t_ast_node	*parse_logical(t_shell_data *shell, t_lex_token **tokens)
{
	t_ast_node	*node;
	t_ast_node	*tmp;

	node = parse_pipeline(shell, tokens);
	if (!node)
		return (NULL);
	while (*tokens && ((*tokens)->type == TOK_AND
			|| (*tokens)->type == TOK_OR))
	{
		if ((*tokens)->type == TOK_AND)
			tmp = new_node(&shell->arena, NODE_AND);
		else
			tmp = new_node(&shell->arena, NODE_OR);
		tmp->left = node;
		*tokens = (*tokens)->next;
		tmp->right = parse_logical(shell, tokens);
		if (!tmp->right)
		{
			shell->last_exit = 2;
			return (NULL);
		}
		node = tmp;
	}
	return (node);
}

t_ast_node	*parse_ast(t_shell_data *shell, t_lex_token **tokens)
{
	t_ast_node	*node;
	t_ast_node	*tmp;

	node = parse_logical(shell, tokens);
	if (!node)
		return (NULL);
	while (*tokens && (*tokens)->type == TOK_SEMICOLON)
	{
		*tokens = (*tokens)->next;
		if (!*tokens)
			return (node);
		tmp = new_node(&shell->arena, NODE_SEQ);
		tmp->left = node;
		tmp->right = parse_ast(shell, tokens);
		if (!tmp->right)
			return (node);
		node = tmp;
	}
	return (node);
}
