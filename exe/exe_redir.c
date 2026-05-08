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

#include "cmd_types.h"
#include "shell.h"
#include "exe_struct.h"
#include "ft_fprintf.h"
#include "libft.h"
#include "expander.h"
#include "parser.h"
#include "arena.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <readline/readline.h>
#include <stdint.h>

static int	setup_redir_fd(t_redirect *r, char *val)
{
	void	*a[1];
	int		fd;

	a[0] = val;
	if (r->type == REDIR_IN)
		fd = open(val, O_RDONLY);
	else if (r->type == REDIR_APPEND)
		fd = open(val, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(val, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		if (errno == ENOENT)
			ft_fprintf(2, "minishell: %s: No such file or directory\n", a);
		else
			ft_fprintf(2, "minishell: %s: Permission denied\n", a);
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
	void	*a[1];

	*valp = expand_word(shell, r->file, &sq);
	a[0] = r->file;
	if (!*valp || (!sq && ft_strchr(*valp, ' ')))
	{
		ft_fprintf(2, "minishell: %s: ambiguous redirect\n", a);
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
		ft_fprintf(2, "minishell: %s: ambiguous redirect\n", a);
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
			fd = open(".heredoc_tmp", O_RDONLY);
			if (fd < 0)
				return (1);
			dup2(fd, STDIN_FILENO);
			close(fd);
			unlink(".heredoc_tmp");
		}
		else if (setup_redir_fd(r, val))
			return (1);
		r = r->next;
	}
	return (0);
}

static void	write_heredoc(int fd, t_redirect *r)
{
	char	*line;
	int		lnum;

	if (!isatty(STDIN_FILENO))
		return ;
	lnum = 1;
	line = readline("");
	while (line)
	{
		if (ft_strncmp(line, r->file, ft_strlen(r->file) + 1) == 0)
		{
			free(line);
			return ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		lnum++;
		line = readline("");
	}
	ft_fprintf(2, "minishell: warning: here-document at line %d"
		" delimited by end-of-file (wanted `%s')\n",
		(void *[]){(void *)(intptr_t)lnum, r->file});
}

void	process_heredoc_list(t_redirect *r)
{
	int	fd;

	while (r)
	{
		if (r->type == REDIR_HEREDOC)
		{
			fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd >= 0)
			{
				write_heredoc(fd, r);
				close(fd);
			}
		}
		r = r->next;
	}
}
