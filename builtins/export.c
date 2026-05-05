/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_fprintf.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include "exe_ctx_utils.h"

static void	sort_env(char **env, int len)
{
	int		i;
	int		j;
	char	*tmp;

	i = -1;
	while (++i < len - 1)
	{
		j = i;
		while (++j < len)
		{
			if (ft_strncmp(env[i], env[j], 1024) > 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
		}
	}
}

static void	print_exported(char **envp)
{
	int		len;
	char	**sorted;
	int		i;
	char	*eq;

	len = 0;
	while (envp[len])
		len++;
	sorted = malloc(sizeof(char *) * (len + 1));
	i = -1;
	while (++i < len)
		sorted[i] = envp[i];
	sorted[len] = NULL;
	sort_env(sorted, len);
	i = -1;
	while (sorted[++i])
	{
		eq = ft_strchr(sorted[i], '=');
		if (eq)
		{
			write(1, "declare -x ", 11);
			write(1, sorted[i], eq - sorted[i]);
			write(1, "=\"", 2);
			write(1, eq + 1, ft_strlen(eq + 1));
			write(1, "\"\n", 2);
		}
		else
		{
			write(1, "declare -x ", 11);
			write(1, sorted[i], ft_strlen(sorted[i]));
			write(1, "\n", 1);
		}
	}
	free(sorted);
}

static int	is_valid_id(char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 0;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	update_env(t_shell *shell, char *arg)
{
	int		i;
	int		j;
	int		len;
	char	*name;
	char	*eq;
	char	**new_envp;

	eq = ft_strchr(arg, '=');
	if (eq)
		name = ft_substr(arg, 0, eq - arg);
	else
		name = ft_strdup(arg);
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], name, ft_strlen(name)) == 0
			&& (shell->envp[i][ft_strlen(name)] == '=' || !shell->envp[i][ft_strlen(name)]))
		{
			free(shell->envp[i]);
			j = i;
			while (shell->envp[j])
			{
				shell->envp[j] = shell->envp[j + 1];
				j++;
			}
		}
		else
			i++;
	}
	len = 0;
	while (shell->envp[len])
		len++;
	new_envp = malloc(sizeof(char *) * (len + 2));
	i = -1;
	while (++i < len)
		new_envp[i] = shell->envp[i];
	new_envp[len] = ft_strdup(arg);
	new_envp[len + 1] = NULL;
	free(shell->envp);
	shell->envp = new_envp;
	free(name);
}

void	free_env(t_shell *shell)
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

void	init_env(t_shell *shell, char **envp)
{
	int		len;
	int		i;
	char	*shlvl;
	char	*new_lvl;
	char	cwd[1024];

	len = 0;
	while (envp[len])
		len++;
	shell->envp = malloc(sizeof(char *) * (len + 1));
	i = -1;
	while (++i < len)
		shell->envp[i] = ft_strdup(envp[i]);
	shell->envp[len] = NULL;
	shlvl = shell_get_env(shell->envp, "SHLVL");
	if (shlvl)
	{
		char *itoa_res = ft_itoa(ft_atoi(shlvl) + 1);
		new_lvl = ft_strjoin("SHLVL=", itoa_res);
		update_env(shell, new_lvl);
		free(new_lvl);
		free(itoa_res);
	}
	else
		update_env(shell, "SHLVL=1");
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

int	builtin_export(t_shell *shell, char **args)
{
	int		i;
	int		ret;
	void	*a[1];

	if (!args[1])
	{
		print_exported(shell->envp);
		return (0);
	}
	ret = 0;
	i = 0;
	while (args[++i])
	{
		if (!is_valid_id(args[i]))
		{
			a[0] = args[i];
			ft_fprintf(2, "minishell: export: `%s': not a valid identifier\n", a);
			ret = 1;
		}
		else
			update_env(shell, args[i]);
	}
	return (ret);
}
