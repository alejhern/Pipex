# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/09 21:50:18 by alejhern          #+#    #+#              #
#    Updated: 2024/11/11 10:16:58 by amhernandez      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIB_DIR = libft/

SRCS = pipex.c pipex_utils.c

OBJ = $(SRCS:.c=.o)

# **************************************************************************** #
#                                 VARIABLES                                    #
# **************************************************************************** #

NAME = pipex

CC = cc

CFLAGS = -Wall -Werror -Wextra

LIBFT = $(LIB_DIR)libft.a

# **************************************************************************** #
#                                 RULES                                        #
# **************************************************************************** #

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIB_DIR) -lft -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I $(LIB_DIR) -c $< -o $@

$(LIBFT):
	@make -C $(LIB_DIR)

clean:
	@make -C $(LIB_DIR) clean
	rm -f $(OBJ)

fclean: clean
	@make -C $(LIB_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

# **************************************************************************** #
#                                BONUS                                         #
# **************************************************************************** #

bonus: $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIB_DIR) -lft -o $(NAME)
