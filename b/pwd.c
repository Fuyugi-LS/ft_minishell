/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"
#include <unistd.h>
#include <limits.h>

/**
 * builtin_pwd - Print the current working directory
 *
 * Uses getcwd. Adheres to Absolute Guarding.
 * Return: 0 on success, 1 on failure
 */
int	builtin_pwd(void)
{
	char	cwd[PATH_MAX];
	void	*args[1];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		args[0] = (void *)cwd;
		ft_fprintf(1, "%s\n", args);
		return (0);
	}
	args[0] = (void *)"pwd error";
	ft_fprintf(2, "minishell: pwd: %s\n", args);
	return (1);
}
