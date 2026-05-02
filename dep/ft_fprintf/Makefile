NAME		= libftfprintf.a

CC			= cc
CFLAGS		= -Wall -Wextra -Werror

SRCS		= ft_fprintf.c \
			  format_str.c \
			  format_utils.c \
			  format_dec.c \
			  format_hex.c \
			  format_uint.c \
			  format_ptr.c

OBJS		= $(SRCS:.c=.o)

GREEN		= \033[0;32m
CYAN		= \033[0;36m
YELLOW		= \033[0;33m
RED			= \033[0;31m
RESET		= \033[0m

all: $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@ -I../libft
	@echo "Compiled: $(CYAN)$<$(RESET)"

$(NAME): $(OBJS)
	@echo "Linking $(NAME)..."
	@ar rcs $(NAME) $(OBJS)
	@echo "$(GREEN)--- $(NAME) successfully built! ---$(RESET)"

clean:
	@echo "Cleaning object files..."
	@rm -f $(OBJS)

fclean: clean
	@echo "Cleaning library..."
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
