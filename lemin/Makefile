# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/02/24 10:26:26 by amakhiny          #+#    #+#              #
#    Updated: 2018/02/24 10:26:30 by amakhiny         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = lem-in

INC = -I ./libft
FLAGS = -Wall -Wextra -Werror -O2
LIB_DIR = libft

SRC_FILES = main.c ants_role.c error_mng.c lem_mng.c lem_player.c lem_read_map.c \
			lem_set_path.c lists_mng.c 

RED		= \033[31m
GREEN	= \033[32m
CYAN	= \033[36m
GREY	= \033[37m

OBJ = $(SRC_FILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	#@echo " $(GREEN)Compiling....."
	@make -C libft
	#@gcc -o $(NAME) $(FLAGS) $(INC) $(OBJ) -lmlx -framework OpenGL -framework AppKit libft/libft.a
	#@gcc -o $(NAME) $(FLAGS) $(INC) $(OBJ) libft/libft.a
	@gcc -o $(NAME) $(INC) $(OBJ) libft/libft.a
	#@echo " $(CYAN)$(NAME) $(GREY)generated \xF0\x9F\x98\xBA"

%.o: %.c
	@gcc $(FLAGS) $(INC) -c -o $@ $<

clean:
	@/bin/rm -f $(OBJ)
	@make -C libft clean
	#@echo " $(RED)All objects has been deleteted!"

fclean: clean
	@/bin/rm -f $(NAME)
	@make -C libft fclean
	#@echo " $(RED)$(NAME)$(GREY) has been deleteted!"

re: fclean all
