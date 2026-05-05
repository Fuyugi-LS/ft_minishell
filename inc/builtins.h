/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "shell.h"

int		builtin_echo(char **args);
int		builtin_pwd(t_shell *shell);
int		builtin_cd(t_shell *shell, char **args);
int		builtin_env(t_shell *shell);
int		builtin_export(t_shell *shell, char **args);
void	update_env(t_shell *shell, char *arg);
void	init_env(t_shell *shell, char **envp);
void	free_env(t_shell *shell);
int		builtin_unset(t_shell *shell, char **args);
void	builtin_exit(t_shell *shell, char **args);

#endif
