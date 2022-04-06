#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_ARGV 1000
# define MAX_HEREDOCS 100
# define SHELL_NAME "gene_shell$ "
# define SYNTAX_ERR "gene_shell: Syntax error"

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "libft/libft.h"
# include "lex.h"
# include <fcntl.h>
# include <stdarg.h>

extern char					**environ;

typedef struct s_cmd
{
	char		*argv[MAX_ARGV];
	int			argv_top;
	int			heredocs_top;
	char		*in_filename;
	char		*out_filename;
	char		*delimeters[MAX_HEREDOCS];
	int			append_mode;
	char		*output;
}	t_cmd;

# define NEXT_PIPELST 0x1
# define NEXT_SCRIPT 0x2

typedef struct s_env_vars
{
	char				*name;
	char				*value;
	struct s_env_vars	*next;
}	t_env_vars;

typedef struct s_pipelist
{
	union {
		t_cmd				*cmd;
		struct s_scripts	*script;
	}	u_item;
	int					type;
	struct s_pipelist	*next;
}	t_pipelist;

typedef struct s_cmd_table
{
	int					logical_op;
	t_pipelist			*pipelist;
	struct s_cmd_table	*next;
}	t_cmd_table;

typedef struct s_scripts
{
	t_cmd_table			*cmd_table;
	struct s_scripts	*next;
}	t_scripts;

typedef struct s_curr_items_ptrs
{
	t_pipelist	*curr_pipelst;
	t_cmd_table	*curr_cmd_table;
	t_scripts	*curr_script;
}	t_curr_items_ptrs;

typedef struct s_env
{
	char		*yytext;
	int			yyleng;
	t_env_vars	*env_vars;
	int			lookahead;
	int			state;
	int			exit_code;
	char		*error_func_name;
	char		*error_custom_msg;
	int			opened_parens;
}	t_env;

# include "get_next_line/get_next_line.h"

void		error(t_env *env);
void		set_err_custom_msg(t_env *env, char *str);
void		set_err_func_name(t_env *env, char *str);
void		advance(t_env *env, int skip_spaces);
int			match(int token, t_env *env);
t_scripts	*statements(t_env *env);
void		close_descriptors(int num_fd, ...);
void		executor(t_scripts *parse_tree, t_env *env, int global_in,
				int global_out);
int			ft_isspace(char c);
int			legal_lookahead(t_env *env, ...);
void		free_parse_tree(t_scripts *parse_tree);

#endif
