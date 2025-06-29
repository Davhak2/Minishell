# Makefile for minishell project

NAME = minishell
SRC_DIR = src
INCLUDE_DIR = includes
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(INCLUDE_DIR)

all:
	@echo "Add build rules here"

clean:
	@echo "Add clean rules here"

fclean: clean
	@echo "Add fclean rules here"

re: fclean all

.PHONY: all clean fclean re
