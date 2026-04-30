/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "cmd_types.h"
# include "arena_struct.h"
# include "shell.h"

t_token	*tokenize_input(t_arena **arena, char *input);
t_cmd	*parse_tokens(t_shell *shell, t_token **tokens, int *cmd_count);
t_node	*parse_ast(t_shell *shell, t_token **tokens);
int		parse_redirection(t_shell *shell, t_redir **redirs, t_token **t);
int		has_wildcard(char *s);
char	*finalize_word(char *s);
void	expand_wildcard(t_shell *shell, char *pat, char ***args, int *wi);

#endif
