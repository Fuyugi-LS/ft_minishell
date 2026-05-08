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
#include "get_next_line.h"
#include "builtins.h"
#include <stdlib.h>

static int	atoll_check_overflow(unsigned long long n, int sign, char c)
{
	if (n > 922337203685477580ULL)
		return (1);
	if (sign == 1 && n == 922337203685477580ULL && (c - '0') > 7)
		return (1);
	if (sign == -1 && n == 922337203685477580ULL && (c - '0') > 8)
		return (1);
	return (0);
}

static int	atoll_parse_digits(const char *str, int *ip,
		unsigned long long *np, int sign)
{
	if (!str[*ip])
		return (1);
	while (str[*ip] >= '0' && str[*ip] <= '9')
	{
		if (atoll_check_overflow(*np, sign, str[*ip]))
			return (1);
		*np = *np * 10 + (str[*ip] - '0');
		(*ip)++;
	}
	while (str[*ip] == ' ' || (str[*ip] >= 9 && str[*ip] <= 13))
		(*ip)++;
	if (str[*ip])
		return (1);
	return (0);
}

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
	if (atoll_parse_digits(str, &i, &n, sign))
		return (1);
	*res = (long long)n * sign;
	return (0);
}

static void	cleanup_and_exit(t_shell_data *shell, int code)
{
	free_env(shell);
	gnl_cleanup();
	arena_destroy(shell->arena);
	exit(code & 0xFF);
}

void	builtin_exit(t_shell_data *shell, char **args)
{
	long long	code;
	int			argc;

	argc = 0;
	while (args[argc])
		argc++;
	if (isatty(STDIN_FILENO))
		write(1, "exit\n", 5);
	if (argc == 1)
		cleanup_and_exit(shell, shell->last_exit);
	if (atoll_check(args[1], &code))
	{
		write(2, "minishell: exit: ", 17);
		write(2, args[1], ft_strlen(args[1]));
		write(2, ": numeric argument required\n", 28);
		cleanup_and_exit(shell, 2);
	}
	if (argc > 2)
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		shell->last_exit = 1;
		return ;
	}
	cleanup_and_exit(shell, code);
}
