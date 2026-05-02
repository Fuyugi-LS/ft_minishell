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

#include <readline/readline.h>
#include <readline/history.h>
#include "shell.h"
#include "signal_minishell.h"
#include "ft_fprintf.h"
#include "parser.h"
#include "exe.h"
#include "libft.h"

int	g_signal = 0;

void	run_loop(t_shell *shell)
{
	char			*input;
	t_token			*tokens;
	t_token			*cur_tok;
	t_node			*ast;

	while (1)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		input = readline("minishell$ ");
		if (!input)
		{
			if (isatty(0))
				write(1, "exit\n", 5);
			break ;
		}
		if (*input)
			add_history(input);
		shell->error_printed = 0;
		tokens = tokenize_input(&shell->arena, input);
		cur_tok = tokens;
		ast = parse_ast(shell, &cur_tok);
		if (!shell->error_printed && (cur_tok || (!ast && shell->last_exit == 2)))
		{
			char *val;
			if (cur_tok)
				val = (cur_tok->type == TOK_ERROR ? "quote" : cur_tok->value);
			else
				val = "newline";
			write(2, "minishell: syntax error near unexpected token `", 47);
			write(2, val, ft_strlen(val));
			write(2, "'\n", 2);
			shell->last_exit = 2;
		}
		if (ast)
			execute_ast(shell, ast);
		free(input);
		arena_reset(shell->arena);
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
#include "builtins.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.envp = NULL;
	shell.last_exit = 0;
	shell.arena = arena_init(ARENA_BLOCK_SIZE);
	if (!shell.arena)
		return (1);
	init_env(&shell, envp);
	init_signals();
	run_loop(&shell);
	arena_destroy(shell.arena);
	return (shell.last_exit);
}
