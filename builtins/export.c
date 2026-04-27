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

static int	env_len(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

/**
 * builtin_export - Set or display environment variables
 * @shell: Shell context
 * @args: args[1..] are KEY=VALUE pairs
 *
 * Return: 0 on success, 1 on failure
 */
int	builtin_export(t_shell *shell, char **args)
{
	int		i;
	int		len;
	char	**new_envp;
	void	*a[1];

	if (!args[1])
	{
		i = -1;
		while (shell->envp[++i])
		{
			a[0] = shell->envp[i];
			ft_fprintf(1, "declare -x %s\n", a);
		}
		return (0);
	}
	len = env_len(shell->envp);
	new_envp = malloc(sizeof(char *) * (len + 2));
	if (!new_envp)
		return (1);
	i = -1;
	while (++i < len)
		new_envp[i] = shell->envp[i];
	new_envp[len] = ft_strdup(args[1]);
	new_envp[len + 1] = NULL;
	shell->envp = new_envp;
	return (0);
}
