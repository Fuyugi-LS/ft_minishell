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

#include <unistd.h>
#include "ft_fprintf.h"
#include "libft.h"

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
	char	buf[4096];
	int		pos;

	i = 1;
	newline = 1;
	while (args[i] && check_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	pos = 0;
	ft_memset(buf, 0, sizeof(buf));
	while (args[i])
	{
		ft_strlcpy(buf + pos, args[i], sizeof(buf) - pos);
		pos += ft_strlen(args[i]);
		if (args[i + 1] && pos < (int)sizeof(buf) - 1)
			buf[pos++] = ' ';
		i++;
	}
	if (newline && pos < (int)sizeof(buf) - 1)
		buf[pos++] = '\n';
	write(1, buf, pos);
	return (0);
}
