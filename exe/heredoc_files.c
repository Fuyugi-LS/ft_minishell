/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_files.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/09/25 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_arena.h"
#include "minishell.h"
#include "ms_parser.h"
#include "libft.h"
#include <stdlib.h>
#include <fcntl.h>

static char	*make_heredoc_path(t_shell_data *shell, int id)
{
	char	*number;
	char	*joined;
	char	*path;

	number = ft_itoa(id);
	if (!number)
		return (NULL);
	joined = ft_strjoin(".heredoc_tmp_", number);
	free(number);
	if (!joined)
		return (NULL);
	path = arena_alloc(&shell->arena, ft_strlen(joined) + 1);
	if (path)
		ft_strlcpy(path, joined, ft_strlen(joined) + 1);
	free(joined);
	return (path);
}

int	open_heredoc_file(t_shell_data *shell, t_redirect *redir, int id)
{
	char	*path;
	int		fd;

	path = make_heredoc_path(shell, id);
	if (!path)
		return (-1);
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd >= 0)
		redir->heredoc_path = path;
	return (fd);
}
