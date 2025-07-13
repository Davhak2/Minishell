NAME					=	minishell

CC						=	cc

RM						=	rm -f

MAKEFILE				=	Makefile

SRC_DIR			=	src/
INC_DIR				=	includes/
INC_FLAGS			=	-Iincludes/ -Iutils/

LIBFT_DIR				=	libft/
LIBFT					=	$(LIBFT_DIR)libft.a

CFLAGS					=	$(INC_FLAGS) -I$(LIBFT_DIR) #-Wall -Wextra -Werror

LDFLAGS					=	-L$(LIBFT_DIR) -lft -lreadline -lncurses

SRC				=	$(SRC_DIR)main.c \
					$(SRC_DIR)signals/signal_handler.c \
					$(SRC_DIR)signals/signal_setup.c \
					$(SRC_DIR)utils/char_utils.c \
					$(SRC_DIR)utils/token_utils.c \
					$(SRC_DIR)utils/lexer_utils.c \
					$(SRC_DIR)utils/parser_utils.c \
					$(SRC_DIR)lexer/lexer.c \
					$(SRC_DIR)parser/parser.c \
							# $(SRC_DIR)parent_monitor.c \
							# $(SRC_DIR)philosopher.c \
							# $(SRC_DIR)routine.c \
							# $(SRC_DIR)semaphores.c \
							# $(SRC_DIR)other.c \
							# $(SRC_DIR)local_semaphores.c

OBJ_DIR					=	obj
OBJ				=	$(SRC:%.c=$(OBJ_DIR)/%.o)

all:					$(NAME)

$(NAME):				$(LIBFT) $(OBJ) $(INC_DIR) $(MAKEFILE)
						$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o:			%.c
						@mkdir -p $(dir $@)
						$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
						$(MAKE) -C $(LIBFT_DIR)

clean:
						$(MAKE) -C $(LIBFT_DIR) clean
						rm -rf $(OBJ_DIR)

fclean:					clean
						$(MAKE) -C $(LIBFT_DIR) fclean
						$(RM) $(NAME)

re:						fclean all

.PHONY:					all clean fclean re libft
