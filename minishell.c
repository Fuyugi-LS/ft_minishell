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

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "shell.h"
#include "signal_minishell.h"
#include "ft_fprintf.h"
#include "parser.h"
#include "exe.h"

int	g_signal = 0;

static void	run_loop(t_shell *shell)
{
	char	*input;
	t_token	*tokens;
	t_cmd	*cmds;
	int		count;

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
		tokens = tokenize_input(&shell->arena, input);
		cmds = parse_tokens(shell, tokens, &count);
		if (cmds)
			execute_commands(shell, cmds, count);
		arena_reset(shell->arena);
		free(input);
	}
}

/**
 * main - Minishell entry point
 * @argc: Argument count
 * @argv: Argument vector
 * @envp: Environment variables
 *
 * Return: last exit status
 */
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.envp = envp;
	shell.last_exit = 0;
	shell.arena = arena_init(ARENA_BLOCK_SIZE);
	if (!shell.arena)
		return (1);
	init_signals();
	run_loop(&shell);
	arena_destroy(shell.arena);
	return (shell.last_exit);
}
