# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/02 13:36:10 by lemmon            #+#    #+#              #
#   Updated: 2022/05/16 14:36:50 by                  ###   ########.fr       # #
#                                                                              #
# **************************************************************************** #

NAME		= minishell
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror
RM			= rm -rf

LIB_DIR    = ./lib/libft/

LIBFT		=	$(addprefix $(LIB_DIR), libft.a)
HEADER		=	sigs.h include/builtins.h include/env_state.h include/env_vars.h \
				include/error.h include/executor.h include/free.h include/lex.h \
				include/libft.h include/parse_tree.h include/parser.h include/tokens.h \
				lib/executor/executor_private.h lib/parser/parser_private.h \
				lib/builtins/builtins_private.h
				
SRCS		=   sigs.c main.c expand_dollar.c lib/lexer/lex.c lib/lexer/no_quotes.c \
				lib/lexer/which_token.c lib/free/clear.c lib/executor/executor.c \
				lib/error/error.c lib/env_vars/env_vars.c lib/parser/command.c \
				lib/parser/expression.c lib/parser/item.c lib/parser/statements.c \
				lib/parser/substring.c lib/parser/substring_dq.c lib/parser/word.c \
				lib/executor/read_heredoc.c lib/builtins/ft_env_unset.c lib/builtins/check_builtins.c \
				lib/executor/exec_pipelist_item.c lib/parser/word_helpers.c \
				lib/executor/find_and_exec_cmd.c lib/executor/fork_and_builtin.c lib/executor/helpers.c lib/executor/iter_pipelist.c \
				main_helpers.c lib/executor/find_and_exec_cmd_utils.c lib/env_vars/set_env_vars.c \
				lib/builtins/cd.c lib/builtins/echo.c lib/builtins/export.c lib/builtins/ft_exit.c \
				lib/builtins/pwd.c

OBJS		= $(SRCS:%.c=%.o)

.PHONY: all clean fclean re bonus

all: $(LIBFT) $(NAME)

$(NAME):	$(OBJS) $(HEADER)
	# @cp $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $@ \
		-lreadline \
		-L/Users/$(USER)/.brew/opt/readline/lib \
		-L/usr/local/opt/readline/lib \
		-I/Users/$(USER)/.brew/opt/readline/include \
		-I/usr/local/opt/readline/include
	
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ \
	-I/Users/$(USER)/.brew/opt/readline/include \
	-I/usr/local/opt/readline/include

$(LIBFT):
	make -C $(LIB_DIR)
	
clean:
	$(RM) $(OBJS)
	make -C $(LIB_DIR) clean

fclean: clean
	$(RM) $(NAME) $(LIBFT)
	
re:	fclean all
