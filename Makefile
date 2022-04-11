CC = gcc
FLAGS = -g -Wall -Werror -Wextra
LIBC = ar
LIBC_FLAGS = -rcs
NAME = minishell

all: $(NAME)

#libft rules
LIBFT = lib/libft/libft.a
$(LIBFT):
	$(MAKE) -C lib/libft

#parser rules
_OBJS_PARSER = command.o expression.o item.o statements.o \
				substring.o substring_dq.o word.o
PARSER_DIR = lib/parser
OBJS_PARSER = $(patsubst %,$(PARSER_DIR)/%,$(_OBJS_PARSER))
_PARSER_LIB = parser.a
PARSER_LIB = $(patsubst %,$(PARSER_DIR)/%,$(_PARSER_LIB))
$(PARSER_LIB): $(OBJS_PARSER)
	$(LIBC) $(LIBC_FLAGS) $(PARSER_LIB) $(OBJS_PARSER)
$(PARSER_DIR)/%.o: $(PARSER_DIR)/%.c
	$(CC) $(FLAGS) -c $^ -o $@

#lexer rules
_OBJS_LEXER = lex.o no_quotes.o which_token.o
LEXER_DIR = lib/lexer
OBJS_LEXER = $(patsubst %,$(LEXER_DIR)/%,$(_OBJS_LEXER))
_LEXER_LIB = lexer.a
LEXER_LIB = $(patsubst %,$(LEXER_DIR)/%,$(_LEXER_LIB))
$(LEXER_LIB): $(OBJS_LEXER)
	$(LIBC) $(LIBC_FLAGS) $(LEXER_LIB) $(OBJS_LEXER)
$(LEXER_DIR)/%.o: $(LEXER_DIR)/%.c
	$(CC) $(FLAGS) -c $^ -o $@

#free rules
OBJS_FREE = lib/free/clear.o
lib/free/clear.o: lib/free/clear.c
	$(CC) $(FLAGS) -c $^ -o $@

#main rules
OBJS_MAIN = main.o sigs.o
$(OBJS_MAIN): $(OBJS_MAIN:.o=.c)
	$(CC) $(FLAGS) -c $^

#error rules
OBJS_ERROR = lib/error/error.o
lib/error/error.o: lib/error/error.c
	$(CC) $(FLAGS) -c $^ -o $@

#env_vars rules
_OBJS_VAR = env_vars.o
VAR_DIR = lib/env_vars
OBJS_VAR = $(patsubst %,$(VAR_DIR)/%,$(_OBJS_VAR))
_VAR_LIB = env_vars.a
VAR_LIB = $(patsubst %,$(VAR_DIR)/%,$(_VAR_LIB))
$(VAR_LIB): $(OBJS_VAR)
	$(LIBC) $(LIBC_FLAGS) $(VAR_LIB) $(OBJS_VAR)
$(VAR_DIR)/%.o: $(VAR_DIR)/%.c
	$(CC) $(FLAGS) -c $^ -o $@

#executor rules
_OBJS_EXEC = executor.o
EXEC_DIR = lib/executor
OBJS_EXEC = $(patsubst %,$(EXEC_DIR)/%,$(_OBJS_EXEC))
_EXEC_LIB = executor.a
EXEC_LIB = $(patsubst %,$(EXEC_DIR)/%,$(_EXEC_LIB))
$(EXEC_LIB): $(OBJS_EXEC)
	$(LIBC) $(LIBC_FLAGS) $(EXEC_LIB) $(OBJS_EXEC)
$(EXEC_DIR)/%.o: $(EXEC_DIR)/%.c
	$(CC) $(FLAGS) -c $^ -o $@

$(NAME): $(PARSER_LIB) $(EXEC_LIB) $(LEXER_LIB) $(VAR_LIB) \
		$(OBJS_ERROR) $(OBJS_FREE) $(OBJS_MAIN) $(LIBFT)
	$(CC) $(OBJS_ERROR) $(OBJS_FREE) $(OBJS_MAIN) $(LEXER_LIB) \
		$(PARSER_LIB) $(EXEC_LIB) $(VAR_LIB) $(LIBFT) -lreadline -o $(NAME)
	
clean:
	/bin/rm -f $(OBJS_PARSER) $(OBJS_EXEC) $(OBJS_LEXER) $(OBJS_VAR) \
		$(OBJS_ERROR) $(OBJS_FREE) $(OBJS_MAIN)
	$(MAKE) clean -C lib/libft

fclean: clean
	/bin/rm -f $(PARSER_LIB) $(EXEC_LIB) $(LEXER_LIB) $(VAR_LIB) $(NAME)
	$(MAKE) fclean -C lib/libft

re: fclean all

.PHONY: all clean fclean re
