/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_types.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_TYPES_H
# define CMD_TYPES_H

typedef enum e_tok_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_REDIR_APPEND,
	TOK_REDIR_HEREDOC,
	TOK_AND,
	TOK_OR,
	TOK_LPAREN,
	TOK_RPAREN,
	TOK_AMPERSAND,
	TOK_SEMICOLON,
	TOK_EOF,
	TOK_ERROR
}	t_tok_type;

typedef struct s_lex_token
{
	t_tok_type			type;
	char				*value;
	struct s_lex_token	*next;
}	t_lex_token;

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

#endif
