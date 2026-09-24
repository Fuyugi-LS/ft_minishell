/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lexer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 20:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/09/24 20:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_LEXER_H
# define MS_LEXER_H

# include "ms_arena.h"

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

t_lex_token	*tokenize_input(t_mem_arena **arena, char *input);
char		*arena_substr(t_mem_arena **a, char *s, int start, int len);
t_tok_type	get_tok_type(char *s);
t_lex_token	*new_lex_token(t_mem_arena **a, char *val, t_tok_type type);

#endif
