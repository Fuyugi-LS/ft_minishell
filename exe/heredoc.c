/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/09/24 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ms_signal.h"
#include "ms_parser.h"
#include "ms_heredoc.h"
#include "libft.h"
#include <readline/readline.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

static int	collect_heredoc(t_shell_data *shell, int fd, t_redirect *redir)
{
	char	*delimiter;
	char	*line;
	int		expand;

	signals_heredoc_mode();
	delimiter = get_heredoc_delimiter(shell, redir->file, &expand);
	if (!delimiter)
		return (1);
	line = read_heredoc_line();
	while (line)
	{
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
			return (free(line), 0);
		if (expand)
			write_heredoc_line(shell, fd, line);
		else
			ft_putendl_fd(line, fd);
		free(line);
		line = read_heredoc_line();
	}
	return (0);
}

static int	wait_heredoc(pid_t pid, struct termios *term, int saved)
{
	int	status;

	if (waitpid(pid, &status, 0) < 0)
		status = 1 << 8;
	if (saved)
		tcsetattr(STDIN_FILENO, TCSANOW, term);
	init_signals();
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			ft_putchar_fd('\n', 1);
		return (128 + WTERMSIG(status));
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

static int	process_one_heredoc(t_shell_data *shell, t_redirect *redir, int id)
{
	struct termios	term;
	pid_t			pid;
	int				fd;
	int				result;
	int				saved;

	fd = open_heredoc_file(shell, redir, id);
	if (fd < 0)
		return (1);
	saved = (tcgetattr(STDIN_FILENO, &term) == 0);
	signals_ignore();
	pid = fork();
	if (pid == 0)
		exit(collect_heredoc(shell, fd, redir));
	close(fd);
	if (pid < 0)
		result = 1;
	else
		result = wait_heredoc(pid, &term, saved);
	if (pid < 0)
		init_signals();
	if (result)
		unlink(redir->heredoc_path);
	return (result);
}

int	process_heredoc_list(t_shell_data *shell, t_redirect *redir, int *id)
{
	int	result;

	result = 0;
	while (redir && !result)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			result = process_one_heredoc(shell, redir, *id);
			(*id)++;
		}
		redir = redir->next;
	}
	return (result);
}
