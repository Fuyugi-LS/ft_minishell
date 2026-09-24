/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 20:00:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/09/24 20:00:00 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_BUILTIN_H
# define MS_BUILTIN_H

# include "minishell.h"

int		builtin_echo(char **args);
int		builtin_pwd(t_shell_data *shell);
int		builtin_cd(t_shell_data *shell, char **args);
int		builtin_env(t_shell_data *shell);
int		builtin_export(t_shell_data *shell, char **args);
int		builtin_unset(t_shell_data *shell, char **args);
void	builtin_exit(t_shell_data *shell, char **args);
int		parse_segments(const char *path, const char **segs, int *lens);
void	build_output(const char **segs, int *lens, int is_abs, char *out);
void	update_pwd(t_shell_data *shell, char *new_pwd);
int		is_builtin(char *name);
int		run_builtin(t_shell_data *shell, char **args);
void	print_exported(char **envp);

#endif
