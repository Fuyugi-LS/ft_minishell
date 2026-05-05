NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g3

SRCS		= minishell.c signals.c \
			  builtins/echo.c builtins/pwd.c builtins/exit.c builtins/cd.c \
			  builtins/env.c builtins/export.c builtins/unset.c \
			  builtins/dispatch.c \
			  mem/arena.c \
			  parser/lexer.c \
			  parser/parser.c \
			  parser/expander.c \
			  parser/wildcard.c \
			  parser/ast.c \
			  exe/executor.c \
			  exe/exe_utils.c \
			  exe/exe_ctx.c \
			  gnl/get_next_line.c \
			  gnl/get_next_line_utils.c
OBJ			= $(SRCS:.c=.o)

FPRINTF_DIR	= dep/ft_fprintf
FPRINTF_LIB	= $(FPRINTF_DIR)/libftfprintf.a

LIBFT_DIR	= dep/libft
LIBFT_LIB	= $(LIBFT_DIR)/libft.a

INC			= -Iinc -Idep/ft_fprintf -Idep/libft -Ignl
LIBS		= -L$(FPRINTF_DIR) -lftfprintf -L$(LIBFT_DIR) -lft -lreadline

all: $(NAME)

$(NAME): $(FPRINTF_LIB) $(LIBFT_LIB) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(FPRINTF_LIB):
	$(MAKE) -C $(FPRINTF_DIR)

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(FPRINTF_DIR) clean
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(FPRINTF_DIR) fclean
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
