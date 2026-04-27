/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "libft.h"

static int	key_matches(char *entry, char *key)
{
	int	klen;

	klen = ft_strlen(key);
	return (ft_strncmp(entry, key, klen) == 0
		&& (entry[klen] == '=' || entry[klen] == '\0'));
}

/**
 * builtin_unset - Remove a variable from the environment
 * @shell: Shell context
 * @args: args[1] is the key to remove
 *
 * Return: 0
 */
int	builtin_unset(t_shell *shell, char **args)
{
	int	i;
	int	j;

	if (!args[1])
		return (0);
	i = 0;
	while (shell->envp[i])
	{
		if (key_matches(shell->envp[i], args[1]))
		{
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
	return (0);
}
