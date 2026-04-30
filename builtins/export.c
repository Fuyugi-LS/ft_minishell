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

static void	update_env(t_shell *shell, char *arg)
{
	int		i;
	int		len;
	char	*name;
	char	*eq;
	char	**new_envp;

	eq = ft_strchr(arg, '=');
	if (eq)
		name = ft_substr(arg, 0, eq - arg);
	else
		name = ft_strdup(arg);
	i = -1;
	while (shell->envp[++i])
	{
		if (ft_strncmp(shell->envp[i], name, ft_strlen(name)) == 0
			&& (shell->envp[i][ft_strlen(name)] == '=' || !shell->envp[i][ft_strlen(name)]))
		{
			if (eq)
			{
				shell->envp[i] = ft_strdup(arg);
			}
			free(name);
			return ;
		}
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
	shell->envp = new_envp;
	free(name);
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
