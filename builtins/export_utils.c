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

#include "minishell.h"
#include "ms_env.h"
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

void	filter_exec_env(char **envp)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
			envp[j++] = envp[i];
		else
			free(envp[i]);
		i++;
	}
	envp[j] = NULL;
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

static void	init_env_pwd(t_shell_data *shell)
{
	char	cwd[1024];
	char	*entry;

	if (shell_get_env(shell->envp, "PWD"))
		return ;
	if (getcwd(cwd, 1024))
	{
		entry = ft_strjoin("PWD=", cwd);
		update_env(shell, entry);
		free(entry);
	}
}

void	init_env(t_shell_data *shell, char **envp)
{
	int		len;
	int		i;

	len = 0;
	while (envp[len])
		len++;
	shell->envp = malloc(sizeof (char *) * (len + 1));
	i = -1;
	while (++i < len)
		shell->envp[i] = ft_strdup(envp[i]);
	shell->envp[len] = NULL;
	init_env_shlvl(shell);
	if (!shell_get_env(shell->envp, "OLDPWD"))
		update_env(shell, "OLDPWD");
	init_env_pwd(shell);
}
