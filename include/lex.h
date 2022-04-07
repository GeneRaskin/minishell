#ifndef LEX_H
# define LEX_H

# define NULL_TOKEN		-1
# define EOI			0
# define SEMI			1
# define OR				2
# define AND			3
# define LP				4
# define RP				5
# define IN_FILE		6
# define OUT_FILE		7
# define HEREDOC		8
# define APPEND_FILE	9
# define PIPE			12
# define SINGLE_QUOTE	13
# define DOUBLE_QUOTE	14
# define DOLLAR_SIGN	15
# define EQUAL_SIGN		16
# define SUBSTRING		17
# define VAR			18
# define SPACE			19
# define NEWLINE		20

# define SINGLE_Q		0x1
# define DOUBLE_Q		0x2
# define DOLLAR			0x4
# define HEREDOC_TK		0x8
# define IN_FILE_TK		0x10
# define OUT_FILE_TK	0x20

# include "global.h"
# include "aux.h"

void	advance(t_env *env, int skip_spaces);
int		match(int token, t_env *env);

#endif
