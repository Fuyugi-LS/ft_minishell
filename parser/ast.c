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

static t_node	*parse_primary(t_shell *shell, t_token **tokens);
static t_node	*parse_pipeline(t_shell *shell, t_token **tokens);
static t_node	*parse_logical(t_shell *shell, t_token **tokens);

static t_node	*new_node(t_arena **a, t_node_type type)
{
	t_node	*n;

	n = arena_alloc(a, sizeof(t_node));
	if (!n)
		return (NULL);
	ft_memset(n, 0, sizeof(t_node));
	n->type = type;
	return (n);
}

static t_node	*parse_primary(t_shell *shell, t_token **tokens)
{
	t_node	*node;

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
		while (*tokens && ((*tokens)->type == TOK_REDIR_IN || (*tokens)->type == TOK_REDIR_OUT 
			|| (*tokens)->type == TOK_REDIR_APPEND || (*tokens)->type == TOK_REDIR_HEREDOC))
		{
			if (parse_redirection(shell, &node->redirs, tokens))
				return (NULL);
		}
		return (node);
	}
	node = new_node(&shell->arena, NODE_COMMAND);
	node->cmds = parse_tokens(shell, tokens, &node->count);
	if (!node->cmds)
		return (NULL);
	return (node);
}

static t_node	*parse_pipeline(t_shell *shell, t_token **tokens)
{
	t_node	*node;
	t_node	*tmp;

	node = parse_primary(shell, tokens);
	if (!node)
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

static t_node	*parse_logical(t_shell *shell, t_token **tokens)
{
	t_node	*node;
	t_node	*tmp;

	node = parse_pipeline(shell, tokens);
	if (!node)
		return (NULL);
	while (*tokens && ((*tokens)->type == TOK_AND || (*tokens)->type == TOK_OR))
	{
		tmp = new_node(&shell->arena, ((*tokens)->type == TOK_AND ? NODE_AND : NODE_OR));
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

t_node	*parse_ast(t_shell *shell, t_token **tokens)
{
	return (parse_logical(shell, tokens));
}
