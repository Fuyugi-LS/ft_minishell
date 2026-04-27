/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_fprintf.h"
#include <unistd.h>

/**
 * builtin_env - Print all environment variables
 * @shell: Shell context
 *
 * Return: 0
 */
int	builtin_env(t_shell *shell)
{
	int		i;
	void	*a[1];

	i = -1;
	while (shell->envp[++i])
	{
		a[0] = shell->envp[i];
		ft_fprintf(1, "%s\n", a);
	}
	return (0);
}
