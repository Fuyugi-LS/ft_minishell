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

t_lex_token	*tokenize_input(t_mem_arena **arena, char *input);
t_command	*parse_tokens(t_shell_data *shell, t_lex_token **tokens,
				int *cmd_count);
t_ast_node	*parse_ast(t_shell_data *shell, t_lex_token **tokens);
int			parse_redirection(t_shell_data *shell, t_redirect **redirs,
				t_lex_token **t);
int			has_wildcard(char *s);
char		*finalize_word(char *s);
int			is_redir_tok(t_lex_token *t);
void		add_redir_node(t_shell_data *shell, t_redirect **list,
				t_redir_kind type, char *file);
void		expand_wildcard(t_shell_data *shell, char *pat,
				char ***args, int *wi);

#endif
