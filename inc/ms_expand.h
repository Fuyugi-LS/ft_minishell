/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 20:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/09/24 20:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_EXPAND_H
# define MS_EXPAND_H

# include "minishell.h"

typedef struct s_word_expand
{
	char	**res;
	char	q;
	int		*seen_q;
}	t_word_expand;

char	*expand_word(t_shell_data *shell, char *s, int *seen_quotes);
char	**expand_cmd_args(t_shell_data *shell, char **args);
int		has_wildcard(char *s);
char	*finalize_word(char *s);
void	expand_wildcard(t_shell_data *shell, char *pat,
			char ***args, int *wi);

#endif
