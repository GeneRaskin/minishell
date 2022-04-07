#ifndef PARSE_TREE_H
# define PARSE_TREE_H

# define MAX_ARGV 1000
# define MAX_HEREDOCS 100
# define NEXT_PIPELST 0x1
# define NEXT_SCRIPT 0x2
# define TAIL 0x1
# define HEAD 0x2

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

typedef struct s_pipelist
{
	union {
		t_cmd				*cmd;
		struct s_scripts	*script;
	}	u_item;
	int					type;
	struct s_pipelist	*next;
	int					pos;
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

#endif
