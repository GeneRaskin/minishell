#include "../../include/builtins.h"
#include "../../include/libft.h"
#include "../../include/env_state.h"
#include "../../include/parse_tree.h"

void	check_builtins(t_cmd *cmd, t_env *env)
{
	size_t	len;

	len = ft_strlen(cmd->argv[0]);
	if (!ft_strncmp(cmd->argv[0], "cd", len))
		cd(cmd, env);
	else if (!ft_strncmp(cmd->argv[0], "env", len))
		ft_env(env->env_vars);
	else if (!ft_strncmp(cmd->argv[0], "pwd", len))
		pwd();
}
