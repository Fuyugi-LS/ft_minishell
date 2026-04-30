/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"
#include <unistd.h>

/**
 * builtin_cd - Change current directory
 * @args: args[1] is the target path
 *
 * Return: 0 on success, 1 on failure
 */
int	builtin_cd(char **args)
{
	void	*a[1];

	if (!args[1])
	{
		ft_fprintf(2, "minishell: cd: missing argument\n", NULL);
		return (1);
	}
	if (args[1] && args[2])
	{
		ft_fprintf(2, "minishell: cd: too many arguments\n", NULL);
		return (1);
	}
	if (chdir(args[1]) == -1)
	{
		a[0] = args[1];
		ft_fprintf(2, "minishell: cd: %s: No such file or directory\n", a);
		return (1);
	}
	return (0);
}
