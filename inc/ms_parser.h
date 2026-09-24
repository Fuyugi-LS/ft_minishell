/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parser.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 20:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/09/24 20:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_PARSER_H
# define MS_PARSER_H

# include "minishell.h"
# include "ms_lexer.h"

typedef enum e_redir_kind
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_kind;

typedef struct s_redirect
{
	t_redir_kind		type;
	char				*file;
	char				*heredoc_path;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_command
{
	char		**args;
	t_redirect	*redirs;
}	t_command;

typedef enum e_node_kind
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL,
	NODE_SEQ
}	t_node_kind;

typedef struct s_ast_node
{
	t_node_kind			type;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	t_command			*cmds;
	t_redirect			*redirs;
	int					count;
}	t_ast_node;

t_command	*parse_tokens(t_shell_data *shell, t_lex_token **tokens,
				int *cmd_count);
t_ast_node	*parse_ast(t_shell_data *shell, t_lex_token **tokens);
int			parse_redirection(t_shell_data *shell, t_redirect **redirs,
				t_lex_token **t);
int			is_redir_tok(t_lex_token *t);
void		add_redir_node(t_shell_data *shell, t_redirect **list,
				t_redir_kind type, char *file);

#endif
