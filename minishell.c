/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/05/07 14:11:27 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "builtins.h"
#include "shell.h"
#include "get_next_line.h"
#include "signal_minishell.h"
#include "ft_fprintf.h"
#include "parser.h"
#include "exe.h"
#include "libft.h"

static char	*get_input_line(void)
{
	char	*input;
	int		ilen;

	write(1, "minishell$ ", 11);
	input = get_next_line(0);
	if (!input)
		return (NULL);
	write(1, "\n", 1);
	ilen = (int) ft_strlen(input);
	if (ilen > 0 && input[ilen - 1] == '\n')
		input[ilen - 1] = '\0';
	return (input);
}

static void	handle_error_message(t_lex_token *cur_tok, t_ast_node *ast,
				t_shell_data *shell)
{
	char	*val;

	val = NULL;
	if (cur_tok)
	{
		if (cur_tok->type == TOK_ERROR)
			val = "quote";
		else
			val = cur_tok->value;
	}
	if (!cur_tok && !ast)
		write(2, "minishell: unexpected end of file\n", 34);
	else if (val)
	{
		write(2, "minishell: syntax error near unexpected token `", 47);
		write(2, val, ft_strlen(val));
		write(2, "'\n", 2);
	}
	shell->last_exit = 2;
}

static void	run_iteration(t_shell_data *shell, char *input)
{
	t_lex_token	*tokens;
	t_lex_token	*cur_tok;
	t_ast_node	*ast;

	shell->error_printed = 0;
	shell->last_input = input;
	tokens = tokenize_input(&shell->arena, input);
	cur_tok = tokens;
	ast = parse_ast(shell, &cur_tok);
	if (!shell->error_printed && (cur_tok || (!ast && shell->last_exit == 2)))
		handle_error_message(cur_tok, ast, shell);
	if (ast && !shell->error_printed && !cur_tok)
	{
		if (traverse_ast_heredocs(ast))
			shell->last_exit = 130;
		else
			execute_ast(shell, ast);
	}
	else if (ast)
		shell->last_exit = 2;
	free(input);
	arena_reset(shell->arena);
}

void	run_loop(t_shell_data *shell)
{
	char	*input;

	while (1)
	{
		if (isatty(0))
		{
			rl_on_new_line();
			rl_replace_line("", 0);
			input = readline("minishell$ ");
			if (!input)
			{
				write(1, "exit\n", 5);
				break ;
			}
			if (*input)
				add_history(input);
		}
		else
		{
			input = get_input_line();
			if (!input)
				break ;
		}
		run_iteration(shell, input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell_data	shell;

	(void)argc;
	(void)argv;
	shell.envp = NULL;
	shell.last_exit = 0;
	shell.last_input = NULL;
	shell.arena = arena_init(ARENA_BLOCK_SIZE);
	if (!shell.arena)
		return (1);
	init_env(&shell, envp);
	init_signals();
	run_loop(&shell);
	free_env(&shell);
	gnl_cleanup();
	arena_destroy(shell.arena);
	return (shell.last_exit);
}
