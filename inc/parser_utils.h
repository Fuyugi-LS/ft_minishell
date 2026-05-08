/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_UTILS_H
# define PARSER_UTILS_H

# include "cmd_types.h"
# include "arena_struct.h"

t_command		*cmd_table_new(t_mem_arena **arena, int count);
char			*arena_substr(t_mem_arena **a, char *s, int start, int len);
t_tok_type		get_tok_type(char *s);
t_lex_token		*new_lex_token(t_mem_arena **a, char *val, t_tok_type type);

#endif
