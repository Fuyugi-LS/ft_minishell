/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "arena.h"

/**
 * struct s_shell - The master shell context passed through the whole program
 * This is the ONE struct that reduces args everywhere.
 */
typedef struct s_shell
{
	char		**envp;
	t_arena		*arena;
	int			last_exit;
}	t_shell;

extern int	g_signal;

#endif
