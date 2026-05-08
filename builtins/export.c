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

void	print_exported(char **envp);

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

static void	append_env(t_shell_data *shell, char *arg)
{
	char	**new_envp;
	int		len;
	int		i;

	len = 0;
	while (shell->envp[len])
		len++;
	new_envp = malloc(sizeof (char *) * (len + 2));
	if (!new_envp)
		return ;
	i = -1;
	while (++i < len)
		new_envp[i] = shell->envp[i];
	new_envp[len] = ft_strdup(arg);
	new_envp[len + 1] = NULL;
	free(shell->envp);
	shell->envp = new_envp;
}

void	update_env(t_shell_data *shell, char *arg)
{
	char	*eq;
	char	*name;
	int		nlen;
	int		i;

	eq = ft_strchr(arg, '=');
	if (eq)
		name = ft_substr(arg, 0, eq - arg);
	else
		name = ft_strdup(arg);
	nlen = (int)ft_strlen(name);
	i = -1;
	while (shell->envp[++i])
	{
		if (ft_strncmp(shell->envp[i], name, nlen) == 0
			&& (shell->envp[i][nlen] == '=' || !shell->envp[i][nlen]))
		{
			free(shell->envp[i]);
			shell->envp[i] = ft_strdup(arg);
			free(name);
			return ;
		}
	}
	free(name);
	append_env(shell, arg);
}

int	builtin_export(t_shell_data *shell, char **args)
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
			ft_fprintf(2,
				"minishell: export: `%s': not a valid identifier\n", a);
			ret = 1;
		}
		else
			update_env(shell, args[i]);
	}
	return (ret);
}
