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

int		exe_ctx_init(t_exec_ctx *ctx);
void	exe_ctx_free(t_exec_ctx *ctx);
char	*exe_get_path(char **envp);
char	*shell_get_env(char **envp, char *key);
void	exe_free_split(char **split);

#endif
