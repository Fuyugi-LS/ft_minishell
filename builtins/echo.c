/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
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
 * check_n_flag - Check if argument is a valid -n flag
 * @arg: The argument string
 *
 * Return: 1 if valid -n (e.g., -n, -nn, -nnn), 0 otherwise
 */
static int	check_n_flag(char *arg)
{
	int	i;

	if (arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1);
}

/**
 * builtin_echo - Emulate bash echo
 * @args: Null-terminated array of arguments (args[0] == "echo")
 *
 * Return: 0 always
 */
int	builtin_echo(char **args)
{
	int		i;
	int		newline;
	void	*prt_args[1];

	i = 1;
	newline = 1;
	while (args[i] && check_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		prt_args[0] = (void *)args[i];
		ft_fprintf(1, "%s", prt_args);
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}
