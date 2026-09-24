/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/09/24 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ms_env.h"
#include "ms_heredoc.h"
#include "libft.h"
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

static int	copy_delimiter_char(char c, char *quote, int *expand)
{
	if (!*quote && (c == '\'' || c == '"'))
	{
		*quote = c;
		*expand = 0;
		return (0);
	}
	if (*quote && c == *quote)
	{
		*quote = 0;
		return (0);
	}
	return (1);
}

char	*get_heredoc_delimiter(t_shell_data *shell, char *word, int *expand)
{
	char	*delimiter;
	char	quote;
	int		i;
	int		j;

	delimiter = arena_alloc(&shell->arena, ft_strlen(word) + 1);
	if (!delimiter)
		return (NULL);
	*expand = 1;
	quote = 0;
	i = 0;
	j = 0;
	while (word[i])
	{
		if (copy_delimiter_char(word[i], &quote, expand))
			delimiter[j++] = word[i];
		i++;
	}
	delimiter[j] = '\0';
	return (delimiter);
}

char	*read_heredoc_line(void)
{
	if (isatty(STDIN_FILENO))
		return (readline("> "));
	return (readline(NULL));
}

static void	write_variable(t_shell_data *shell, int fd, char *line, int *i)
{
	char	*name;
	char	*value;
	int		start;

	start = ++(*i);
	if (line[start] == '?')
	{
		value = ft_itoa(shell->last_exit);
		ft_putstr_fd(value, fd);
		free(value);
		(*i)++;
		return ;
	}
	while (ft_isalnum(line[*i]) || line[*i] == '_')
		(*i)++;
	if (start == *i)
		return (ft_putchar_fd('$', fd));
	name = ft_substr(line, start, *i - start);
	value = shell_get_env(shell->envp, name);
	if (value)
		ft_putstr_fd(value, fd);
	free(name);
}

void	write_heredoc_line(t_shell_data *shell, int fd, char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
			write_variable(shell, fd, line, &i);
		else
			ft_putchar_fd(line[i++], fd);
	}
	ft_putchar_fd('\n', fd);
}
