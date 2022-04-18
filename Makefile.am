AM_CFLAGS = -Wall -Wextra -Werror

bin_PROGRAMS = minishell
minishell_SOURCES = sigs.c sigs.h main.c lib/lexer/lex.c lib/lexer/no_quotes.c \
				lib/lexer/which_token.c lib/free/clear.c lib/executor/executor.c \
				lib/error/error.c lib/env_vars/env_vars.c lib/parser/command.c \
				lib/parser/expression.c lib/parser/item.c lib/parser/statements.c \
				lib/parser/substring.c lib/parser/substring_dq.c lib/parser/word.c \
				lib/executor/read_heredoc.c

minishell_LDADD = -lreadline lib/libft/libft.a

clean-local:
	@rm config.status configure config.log
	@rm Makefile
	@rm aclocal.m4
	@rm -r autom4te.cache/
	@rm compile install-sh missing Makefile.in