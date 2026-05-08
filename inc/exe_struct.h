/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_struct.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_STRUCT_H
# define EXE_STRUCT_H

# include "cmd_types.h"
# include "shell.h"
# include <sys/types.h>

typedef struct s_exec_context
{
	t_command		*cmds;
	int				count;
	t_shell_data	*shell;
	pid_t			*pids;
	int				(*pipes)[2];
}	t_exec_context;

#endif
