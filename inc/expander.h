/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "shell.h"
# include "cmd_types.h"

typedef struct s_word_expand
{
	char	**res;
	char	q;
	int		*seen_q;
}	t_word_expand;

char	*expand_word(t_shell_data *shell, char *s, int *seen_quotes);
int		setup_redirs(t_shell_data *shell, t_redirect *r);
char	**expand_cmd_args(t_shell_data *shell, char **args);

#endif
