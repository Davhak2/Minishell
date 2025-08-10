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
					$(SRC_DIR)utils/lexer/lexer_utils1.c \
					$(SRC_DIR)utils/lexer/lexer_utils2.c \
					$(SRC_DIR)utils/parser/parser_utils1.c \
					$(SRC_DIR)utils/parser/parser_utils2.c \
					$(SRC_DIR)utils/free_shell.c \
					$(SRC_DIR)lexer/lexer1.c \
					$(SRC_DIR)lexer/lexer2.c \
					$(SRC_DIR)lexer/lexer3.c \
					$(SRC_DIR)parser/parser1.c \
					$(SRC_DIR)parser/parser2.c \
					$(SRC_DIR)parser/parser3.c \
					$(SRC_DIR)expander/expander.c \
					$(SRC_DIR)executor/executor.c \
					$(SRC_DIR)builtins/ft_pwd.c \
					$(SRC_DIR)builtins/ft_echo.c \
					$(SRC_DIR)builtins/cd/ft_cd1.c \
					$(SRC_DIR)builtins/cd/ft_cd2.c \
					$(SRC_DIR)builtins/ft_exit.c \
					$(SRC_DIR)builtins/export/ft_export1.c \
					$(SRC_DIR)builtins/export/ft_export2.c \
					$(SRC_DIR)builtins/export/ft_export3.c \
					$(SRC_DIR)builtins/export/ft_export4.c \
					$(SRC_DIR)utils/builtins/builtin_utils1.c \
					$(SRC_DIR)utils/builtins/builtin_utils2.c \
					$(SRC_DIR)utils/builtins/builtin_utils3.c \
					$(SRC_DIR)builtins/ft_env.c \
					$(SRC_DIR)builtins/ft_unset.c

OBJ_DIR					=	obj
OBJ						=	$(SRC:%.c=$(OBJ_DIR)/%.o)

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
						valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=ignore_readline.supp ./minishell

.PHONY:					all clean fclean re libft
