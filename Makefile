NAME        = minishell

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g3

SRCS        = minishell.c signals.c signal_modes.c print_error.c \
              builtins/echo.c builtins/pwd.c builtins/exit.c builtins/cd.c \
              builtins/cd_utils.c \
              builtins/env.c builtins/export.c builtins/export_print.c \
              builtins/export_utils.c builtins/unset.c \
              builtins/dispatch.c \
              mem/arena.c \
              parser/lexer.c \
              parser/lexer_utils.c \
              parser/parser.c \
              parser/parser_misc.c \
              parser/expander.c \
              parser/wildcard.c \
              parser/ast.c \
              exe/executor.c \
              exe/exe_exec_helpers.c \
              exe/exe_utils.c \
              exe/exe_ctx.c \
              exe/exe_redir.c \
              exe/heredoc.c \
              exe/heredoc_expand.c \
              exe/heredoc_files.c \
              exe/heredoc_tree.c \
              exe/exe_wait.c \
              exe/exe_child.c
OBJ         = $(SRCS:.c=.o)

LIBFT_DIR   = libft
LIBFT_LIB   = $(LIBFT_DIR)/libft.a

INC         = -Iinc -Ilibft
LIBS        = -L$(LIBFT_DIR) -lft -lreadline

all: $(NAME)

$(NAME): $(LIBFT_LIB) $(OBJ)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)
	@echo "Built $(NAME) successfully!"

norm:
	norminette $(SRCS) inc libft

%.o: %.c
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@ > /dev/null

$(LIBFT_LIB):
	@echo "Building libft"
	@$(MAKE) -s -C $(LIBFT_DIR) > /dev/null 2>&1

clean:
	@echo "Cleaning object and dependency"
	@rm -f $(OBJ)
	@$(MAKE) -s -C $(LIBFT_DIR) fclean > /dev/null 2>&1

fclean: clean
	@echo "Removing $(NAME) and libraries"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re norm
