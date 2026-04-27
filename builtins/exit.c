/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
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

/**
 * builtin_exit - Exit the shell with optional code
 * @shell: Shell context
 * @args: args[1] optional numeric exit code
 */
void	builtin_exit(t_shell *shell, char **args)
{
	int		code;
	void	*a[1];

	ft_fprintf(1, "exit\n", NULL);
	code = shell->last_exit;
	if (args[1])
	{
		if (!ft_isdigit(args[1][0]) && args[1][0] != '-')
		{
			a[0] = args[1];
			ft_fprintf(2, "minishell: exit: %s: numeric argument required\n", a);
			code = 2;
		}
		else
			code = ft_atoi(args[1]) & 0xFF;
	}
	arena_destroy(shell->arena);
	exit(code);
}
