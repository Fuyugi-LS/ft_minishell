/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_tree.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/09/25 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ms_parser.h"
#include "ms_heredoc.h"
#include <unistd.h>

static void	cleanup_redir_list(t_redirect *redir)
{
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC && redir->heredoc_path)
			unlink(redir->heredoc_path);
		redir = redir->next;
	}
}

void	cleanup_ast_heredocs(t_ast_node *node)
{
	int	i;

	if (!node)
		return ;
	if (node->type == NODE_COMMAND)
	{
		i = -1;
		while (++i < node->count)
			cleanup_redir_list(node->cmds[i].redirs);
	}
	cleanup_redir_list(node->redirs);
	cleanup_ast_heredocs(node->left);
	cleanup_ast_heredocs(node->right);
}

static int	traverse_heredocs(t_shell_data *shell, t_ast_node *node, int *id)
{
	int	i;
	int	result;

	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
	{
		i = -1;
		while (++i < node->count)
		{
			result = process_heredoc_list(shell, node->cmds[i].redirs, id);
			if (result)
				return (result);
		}
		return (0);
	}
	result = process_heredoc_list(shell, node->redirs, id);
	if (!result)
		result = traverse_heredocs(shell, node->left, id);
	if (!result)
		result = traverse_heredocs(shell, node->right, id);
	return (result);
}

int	traverse_ast_heredocs(t_shell_data *shell, t_ast_node *node)
{
	int	id;

	id = 0;
	return (traverse_heredocs(shell, node, &id));
}
