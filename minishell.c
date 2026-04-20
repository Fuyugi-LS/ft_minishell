/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "signals.h"
#include "dep/ft_fprintf.h"

/**
 * main - The entry point for Minishell
 * @argc: Argument count
 * @argv: Argument vector
 * @envp: Environment variables
 *
 * Return: 0 on success, or an error code
 */
int	main(int argc, char **argv, char **envp)
{
	char	*input;

	(void)argc;
	(void)argv;
	(void)envp;
	init_signals();
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			ft_fprintf(1, "exit\n", NULL);
			break ;
		}
		if (*input)
			add_history(input);
		free(input);
	}
	return (0);
}
