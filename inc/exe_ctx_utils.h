/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_ctx_utils.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_CTX_UTILS_H
# define EXE_CTX_UTILS_H

# include "exe_struct.h"

int		exe_context_init(t_exec_context *context, t_command *cmds,
			int count, t_shell_data *shell);
void	exe_context_free(t_exec_context *context);
char	*exe_get_path(char **envp);
char	*shell_get_env(char **envp, char *key);
void	exe_free_split(char **split);
void	print_exported(char **envp);
void	child_cleanup(t_shell_data *shell);
int		setup_redirs(t_shell_data *shell, t_redirect *r);
void	execute_ast(t_shell_data *shell, t_ast_node *node);
void	close_all_pipes(int (*pipes)[2], int count);
void	run_pipe_child(t_shell_data *shell, t_ast_node *node, int *p, int side);
void	run_child(t_exec_context *context, int index);
int		exec_builtin_single(t_shell_data *shell, t_command *cmds, int count);
void	update_underscore(t_shell_data *shell, char **args);
int		process_heredoc_list(t_redirect *r);
void	close_extra_fds(void);

#endif
