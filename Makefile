NAME					=	minishell

CC						=	cc

RM						=	rm -f

MAKEFILE				=	Makefile

SRC_DIR			=	src/
INC_DIR				=	includes/
INC_FLAGS			=	-Iincludes/ -Iutils/

LIBFT_DIR				=	libft/
LIBFT					=	$(LIBFT_DIR)libft.a

CFLAGS					=	-g $(INC_FLAGS) -I$(LIBFT_DIR) #-Wall -Wextra -Werror

LDFLAGS					=	-L$(LIBFT_DIR) -lft -lreadline -lncurses

SRC				=	$(SRC_DIR)main.c \
					$(SRC_DIR)signals/signal_handler.c \
					$(SRC_DIR)signals/signal_setup.c \
					$(SRC_DIR)utils/char_utils.c \
					$(SRC_DIR)utils/token_utils.c \
					$(SRC_DIR)utils/lexer_utils.c \
					$(SRC_DIR)utils/parser_utils.c \
					$(SRC_DIR)utils/free_shell.c \
					$(SRC_DIR)lexer/lexer.c \
					$(SRC_DIR)parser/parser.c \
					$(SRC_DIR)expander/expander.c \
					$(SRC_DIR)executor/executor.c \
					$(SRC_DIR)builtins/ft_pwd.c \
					$(SRC_DIR)builtins/ft_echo.c \
					$(SRC_DIR)builtins/ft_cd.c \
					$(SRC_DIR)builtins/ft_exit.c \
					$(SRC_DIR)builtins/ft_export.c \
					$(SRC_DIR)utils/builtin_utils.c \
					$(SRC_DIR)builtins/ft_env.c 

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

valgrind:
						valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=ignore_readline.supp ./minishell

.PHONY:					all clean fclean re libft
