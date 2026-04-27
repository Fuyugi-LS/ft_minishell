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
 * builtin_pwd - Print working directory
 *
 * Return: 0 on success, 1 on failure
 */
int	builtin_pwd(void)
{
	char	buf[PATH_MAX];
	void	*a[1];

	if (!getcwd(buf, PATH_MAX))
	{
		ft_fprintf(2, "minishell: pwd: getcwd failed\n", NULL);
		return (1);
	}
	a[0] = buf;
	ft_fprintf(1, "%s\n", a);
	return (0);
}
