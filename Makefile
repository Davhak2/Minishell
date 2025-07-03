NAME					=	minishell

CC						=	cc

RM						=	rm -f

MAKEFILE				=	Makefile

SRC_DIR			=	src/

INC_DIR				=	includes/

CFLAGS					=	-I$(INC_DIR) #-Wall -Wextra -Werror
LDFLAGS  = -lreadline -lncurses

SRC				=	$(SRC_DIR)main.c \
					$(SRC_DIR)/signals/signal_handler.c \
					$(SRC_DIR)/signals/signal_setup.c \
							# $(SRC_DIR)monitor.c \
							# $(SRC_DIR)parent_monitor.c \
							# $(SRC_DIR)philosopher.c \
							# $(SRC_DIR)routine.c \
							# $(SRC_DIR)semaphores.c \
							# $(SRC_DIR)other.c \
							# $(SRC_DIR)local_semaphores.c

OBJ_DIR					=	obj
OBJ				=	$(SRC:%.c=$(OBJ_DIR)/%.o)

all:					$(NAME)

$(NAME):		$(OBJ) $(INC_DIR) $(MAKEFILE)
						$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o:			%.c
						@mkdir -p $(dir $@)
						$(CC) $(CFLAGS) -c $< -o $@

clean:
						rm -rf $(OBJ_DIR)

fclean:					clean
						$(RM) $(NAME)

re:						fclean all

.PHONY:					all clean fclean re
