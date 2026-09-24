/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 20:00:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/09/24 20:00:00 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_ENV_H
# define MS_ENV_H

# include "minishell.h"

char	*shell_get_env(char **envp, char *key);
void	update_env(t_shell_data *shell, char *arg);
void	init_env(t_shell_data *shell, char **envp);
void	free_env(t_shell_data *shell);
void	filter_exec_env(char **envp);

#endif
