NAME        = minishell

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g3

SRCS        = minishell.c signals.c \
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
              exe/exe_child.c \
              gnl/get_next_line.c \
              gnl/get_next_line_utils.c
OBJ         = $(SRCS:.c=.o)

FPRINTF_DIR = dep/ft_fprintf
FPRINTF_LIB = $(FPRINTF_DIR)/libftfprintf.a

LIBFT_DIR   = dep/libft
LIBFT_LIB   = $(LIBFT_DIR)/libft.a

INC         = -Iinc -Idep/ft_fprintf -Idep/libft -Ignl
LIBS        = -L$(FPRINTF_DIR) -lftfprintf -L$(LIBFT_DIR) -lft -lreadline

all: norm $(NAME)

$(NAME): $(FPRINTF_LIB) $(LIBFT_LIB) $(OBJ)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)
	@echo "Built $(NAME) successfully!"

norm:
	@echo "Checking Norminette"
	@norminette * 2>&1 | grep "Error!" && \
		(echo "Norminette ERROR! Compilation aborted." && exit 1) || true

%.o: %.c
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@ > /dev/null

$(FPRINTF_LIB):
	@echo "Building ft_fprintf"
	@$(MAKE) -s -C $(FPRINTF_DIR) > /dev/null 2>&1

$(LIBFT_LIB):
	@echo "Building libft"
	@$(MAKE) -s -C $(LIBFT_DIR) > /dev/null 2>&1

clean:
	@echo "Cleaning object and dependency"
	@rm -f $(OBJ)
	@$(MAKE) -s -C $(FPRINTF_DIR) fclean > /dev/null 2>&1
	@$(MAKE) -s -C $(LIBFT_DIR) fclean > /dev/null 2>&1

fclean: clean
	@echo "Removing $(NAME) and libraries"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re norm
