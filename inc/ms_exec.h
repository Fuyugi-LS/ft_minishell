/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 20:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/09/24 20:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_EXEC_H
# define MS_EXEC_H

# include <sys/types.h>
# include "minishell.h"
# include "ms_parser.h"

typedef struct s_exec_context
{
	t_command		*cmds;
	int				count;
	t_shell_data	*shell;
	pid_t			*pids;
	int				(*pipes)[2];
}	t_exec_context;

void	execute_commands(t_shell_data *shell, t_command *cmds, int count);
void	execute_ast(t_shell_data *shell, t_ast_node *node);
int		exe_context_init(t_exec_context *context, t_command *cmds,
			int count, t_shell_data *shell);
void	exe_context_free(t_exec_context *context);
char	*exe_get_path(char **envp);
void	exe_free_split(char **split);
void	child_cleanup(t_shell_data *shell);
int		setup_redirs(t_shell_data *shell, t_redirect *r);
void	close_all_pipes(int (*pipes)[2], int count);
void	run_pipe_child(t_shell_data *shell, t_ast_node *node, int *p, int side);
void	run_child(t_exec_context *context, int index);
int		exec_builtin_single(t_shell_data *shell, t_command *cmds, int count);
void	update_underscore(t_shell_data *shell, char **args);
void	apply_wait_status(t_shell_data *shell, int status);
void	close_extra_fds(void);
void	exe_launch(t_command *cmd, t_shell_data *shell);

#endif
