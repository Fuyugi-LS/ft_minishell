/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 20:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/09/24 20:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_HEREDOC_H
# define MS_HEREDOC_H

# include "minishell.h"
# include "ms_parser.h"

int		traverse_ast_heredocs(t_shell_data *shell, t_ast_node *node);
int		process_heredoc_list(t_shell_data *shell, t_redirect *r, int *id);
int		open_heredoc_file(t_shell_data *shell, t_redirect *redir, int id);
void	cleanup_ast_heredocs(t_ast_node *node);
char	*get_heredoc_delimiter(t_shell_data *shell, char *word, int *expand);
char	*read_heredoc_line(void);
void	write_heredoc_line(t_shell_data *shell, int fd, char *line);

#endif
