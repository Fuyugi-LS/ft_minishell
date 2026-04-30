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

static int	atoll_check(const char *str, long long *res)
{
	unsigned long long	n;
	int					sign;
	int					i;

	n = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!str[i])
		return (1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (sign == 1 && (n > 922337203685477580ULL || (n == 922337203685477580ULL && (str[i] - '0') > 7)))
			return (1);
		if (sign == -1 && (n > 922337203685477580ULL || (n == 922337203685477580ULL && (str[i] - '0') > 8)))
			return (1);
		n = n * 10 + (str[i] - '0');
		i++;
	}
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i])
		return (1);
	*res = n * sign;
	return (0);
}

void	builtin_exit(t_shell *shell, char **args)
{
	long long	code;
	int			argc;

	argc = 0;
	while (args[argc])
		argc++;
	if (isatty(STDIN_FILENO))
		write(1, "exit\n", 5);
	if (argc == 1)
	{
		arena_destroy(shell->arena);
		exit(shell->last_exit & 0xFF);
	}
	if (atoll_check(args[1], &code))
	{
		write(2, "minishell: exit: ", 17);
		write(2, args[1], ft_strlen(args[1]));
		write(2, ": numeric argument required\n", 28);
		arena_destroy(shell->arena);
		exit(2);
	}
	if (argc > 2)
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		shell->last_exit = 1;
		return ;
	}
	arena_destroy(shell->arena);
	exit(code & 0xFF);
}
