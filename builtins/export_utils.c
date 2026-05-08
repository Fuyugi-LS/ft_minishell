/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "builtins.h"
#include "exe_ctx_utils.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

void	free_env(t_shell_data *shell)
{
	int	i;

	if (!shell->envp)
		return ;
	i = 0;
	while (shell->envp[i])
	{
		free(shell->envp[i]);
		i++;
	}
	free(shell->envp);
	shell->envp = NULL;
}

static void	init_env_shlvl(t_shell_data *shell)
{
	char	*shlvl;
	char	*itoa_res;
	char	*new_lvl;

	shlvl = shell_get_env(shell->envp, "SHLVL");
	if (shlvl)
	{
		itoa_res = ft_itoa(ft_atoi(shlvl) + 1);
		new_lvl = ft_strjoin("SHLVL=", itoa_res);
		update_env(shell, new_lvl);
		free(new_lvl);
		free(itoa_res);
	}
	else
		update_env(shell, "SHLVL=1");
}

void	init_env(t_shell_data *shell, char **envp)
{
	int		len;
	int		i;
	char	cwd[1024];
	char	*new_lvl;

	len = 0;
	while (envp[len])
		len++;
	shell->envp = malloc(sizeof (char *) * (len + 1));
	i = -1;
	while (++i < len)
		shell->envp[i] = ft_strdup(envp[i]);
	shell->envp[len] = NULL;
	init_env_shlvl(shell);
	if (!shell_get_env(shell->envp, "PWD"))
	{
		if (getcwd(cwd, 1024))
		{
			new_lvl = ft_strjoin("PWD=", cwd);
			update_env(shell, new_lvl);
			free(new_lvl);
		}
	}
}
