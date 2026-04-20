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

#include "ft_fprintf.h"
#include <stdlib.h>

/**
 * builtin_exit - Exit the shell
 * @args: Arguments passed to exit
 *
 * Will be expanded later to handle numeric validation
 * and exactly replicated bash exit codes.
 *
 * Return: 0 (though it usually exits before returning)
 */
int	builtin_exit(char **args)
{
	(void)args;
	ft_fprintf(1, "exit\n", NULL);
	exit(0);
	return (0);
}
