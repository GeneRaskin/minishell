#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "global.h"
# include "parse_tree.h"
# include "aux.h"

extern char	**environ;

void	executor(t_scripts *parse_tree, t_env *env,
			int global_in, int global_out);

#endif