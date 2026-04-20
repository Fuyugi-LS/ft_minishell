NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g3

SRC			= minishell.c signals.c \
			  b/echo.c b/pwd.c b/exit.c b/cd.c b/env.c b/export.c b/unset.c
OBJ			= $(SRC:.c=.o)

FPRINTF_DIR	= dep/ft_fprintf
FPRINTF_LIB	= $(FPRINTF_DIR)/libftfprintf.a

INC			= -Iinc -Idep/ft_fprintf
LIBS		= -L$(FPRINTF_DIR) -lftfprintf -lreadline

all: $(NAME)

$(NAME): $(FPRINTF_LIB) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(FPRINTF_LIB):
	$(MAKE) -C $(FPRINTF_DIR)

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(FPRINTF_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(FPRINTF_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
