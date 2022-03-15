#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

void	executor(t_scripts *parse_tree, t_env *env, int global_in,
			int global_out);

#endif
