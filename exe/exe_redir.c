/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_arena.h"
#include "minishell.h"
#include "ms_parser.h"
#include "ms_expand.h"
#include "ms_exec.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>

static int	setup_redir_fd(t_redirect *r, char *val)
{
	int		fd;

	if (r->type == REDIR_IN)
		fd = open(val, O_RDONLY);
	else if (r->type == REDIR_APPEND)
		fd = open(val, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(val, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		if (errno == ENOENT)
			print_error("minishell: ", val,
				": No such file or directory");
		else
			print_error("minishell: ", val, ": Permission denied");
		return (1);
	}
	if (r->type == REDIR_IN)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	expand_redir_target(t_shell_data *shell, t_redirect *r, char **valp)
{
	char	**wild_args;
	int		wi;
	int		sq;

	*valp = expand_word(shell, r->file, &sq);
	if (!*valp || (!sq && ft_strchr(*valp, ' ')))
	{
		print_error("minishell: ", r->file, ": ambiguous redirect");
		return (1);
	}
	wild_args = arena_alloc(&shell->arena, sizeof (char *) * 1024);
	wi = 0;
	if (has_wildcard(*valp))
		expand_wildcard(shell, *valp, &wild_args, &wi);
	else
		wild_args[wi++] = *valp;
	if (wi != 1)
	{
		print_error("minishell: ", r->file, ": ambiguous redirect");
		return (1);
	}
	*valp = finalize_word(wild_args[0]);
	return (0);
}

int	setup_redirs(t_shell_data *shell, t_redirect *r)
{
	char	*val;
	int		fd;

	while (r)
	{
		val = NULL;
		if (r->type != REDIR_HEREDOC)
		{
			if (expand_redir_target(shell, r, &val))
				return (1);
		}
		if (r->type == REDIR_HEREDOC)
		{
			fd = open(r->heredoc_path, O_RDONLY);
			if (fd < 0)
				return (1);
			dup2(fd, STDIN_FILENO);
			close(fd);
			unlink(r->heredoc_path);
		}
		else if (setup_redir_fd(r, val))
			return (1);
		r = r->next;
	}
	return (0);
}
