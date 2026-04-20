# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vasukmua <vasukmua@student.42bangkok.com>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/20 11:34:24 by vasukmua          #+#    #+#              #
#    Updated: 2026/04/20 11:36:04 by vasukmua         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all, clean, fclean, bonus, re

ARNAME = libft.a
COMPILER = cc
FLAGS = -Wall -Wextra -Werror
SRC1 = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c \
		ft_isprint.c ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c \
		ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c \
		ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c \
		ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c
SRC2 = ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
		ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c \
		ft_putendl_fd.c ft_putnbr_fd.c
SRC_BONUS = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c \
			ft_lstadd_back_bonus.c ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c \
			ft_lstmap_bonus.c
OBJ = $(SRC1:.c=.o) $(SRC2:.c=.o)
OBJ_BONUS = $(SRC_BONUS:.c=.o)

all: $(ARNAME)

bonus: $(OBJ_BONUS) $(ARNAME)
	ar rcs $(ARNAME) $(OBJ_BONUS)

$(ARNAME): $(OBJ)
	ar rcs $(ARNAME) $(OBJ)

%.o: %.c
	$(COMPILER) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(OBJ_BONUS)

fclean: clean
	rm -rf $(ARNAME)

re: fclean all
