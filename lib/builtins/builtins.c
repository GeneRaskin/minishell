#include "../../include/libft.h"
#include <stdio.h>
#include "../../include/env_state.h"
#include "../../include/error.h"
#include "../../include/parse_tree.h"
#include "../../include/env_vars.h"
#include "../../include/free.h"
#include <readline/history.h>
#include <sys/param.h>

void	cd(t_cmd *cmd, t_env *env)
{
	char	*pwd_value;
	char	buf[MAXPATHLEN];

	if (cmd->argv_top > 1)
	{
		env->error_custom_msg = CD_ARGC_ERROR;
		env->exit_code = EXIT_FAILURE;
		return ;
	}
	if (chdir(cmd->argv[1]) == -1)
	{
		env->error_func_name = "cd";
		env->exit_code = EXIT_FAILURE;
		return ;
	}
	pwd_value = ft_strjoin("PWD=", getcwd(buf, MAXPATHLEN));
	set("PWD", pwd_value, &(env->global_env_vars), env);
	env->exit_code = EXIT_SUCCESS;
}

void	pwd(t_cmd *cmd, t_env *env)
{
	char	buf[MAXPATHLEN];

	if (cmd->argv_top != 0)
	{
		env->error_custom_msg = PWD_ARGC_ERROR;
		env->exit_code = EXIT_FAILURE;
		return ;
	}
	getcwd(buf, MAXPATHLEN);
	ft_putstr_fd(buf, STDOUT_FILENO);
	write(STDOUT_FILENO, "\n", 1);
	env->exit_code = EXIT_SUCCESS;
}

void	ft_env(t_cmd *cmd, t_env *env)
{
	t_env_vars	*vars;

	if (cmd->argv_top != 0)
	{
		env->error_custom_msg = ENV_ARGC_ERROR;
		env->exit_code = EXIT_FAILURE;
		return ;
	}
	vars = env->global_env_vars;
	while (vars != NULL)
	{
		printf("%s=%s\n", vars->name, vars->value);
		vars = vars->next;
	}
	env->exit_code = EXIT_SUCCESS;
}

static char	**get_vars_values(char *arg)
{
	char	**vars_values;
	int		var_len;
	int		val_len;

	var_len = 0;
	while (arg[var_len] && arg[var_len] != '=')
		var_len++;
	if (!arg[var_len])
		vars_values = (char **) malloc(sizeof(char *) * 2);
	else
		vars_values = (char **) malloc(sizeof(char *) * 3);
	vars_values[0] = ft_substr(arg, 0, var_len);
	if (!arg[var_len])
		vars_values[1] = NULL;
	else
	{
		val_len = 0;
		while (arg[var_len + 1 + val_len])
			val_len++;
		vars_values[1] = ft_substr(arg, var_len + 1, val_len);
		vars_values[2] = NULL;
	}
	return (vars_values);
}

void	export(t_cmd *cmd, t_env *env)
{
	int		i;
	char	**vars_values;
	char	*value;

	i = 1;
	if (cmd->argv_top == 0)
	{
		ft_env(cmd, env);
		return ;
	}
	else
	{
		while (i <= cmd->argv_top)
		{
			vars_values = get_vars_values(cmd->argv[i]);
			if (vars_values[1])
			{
				value = get(vars_values[0], env->global_env_vars);
				set(vars_values[0], vars_values[1],
					&(env->global_env_vars), env);
				if (get(vars_values[0], env->env_vars))
					unset(vars_values[0], &(env->env_vars));
				if (value)
					free(vars_values[0]);
				free(vars_values);
			}
			else
			{
				value = get(vars_values[0], env->env_vars);
				if (value)
				{
					value = ft_strdup(value);
					unset(vars_values[0], &(env->env_vars));
					set(vars_values[0], value, &(env->global_env_vars), env);
				}
				else
				{
					value = get(vars_values[0], env->global_env_vars);
					if (!value)
						set(vars_values[0], ft_strdup(""),
							&(env->global_env_vars), env);
					else
						free(vars_values[0]);
				}
				free(vars_values);
			}
			i++;
		}
	}
	env->exit_code = EXIT_SUCCESS;
}

void	ft_unset(t_cmd *cmd, t_env *env)
{
	int	i;

	i = 1;
	if (cmd->argv_top < 1)
	{
		env->error_custom_msg = UNSET_ARGC_ERROR;
		env->exit_code = EXIT_FAILURE;
		return ;
	}
	while (i <= cmd->argv_top)
	{
		if (get(cmd->argv[i], env->global_env_vars))
			unset(cmd->argv[i], &(env->global_env_vars));
		else
			unset(cmd->argv[i], &(env->env_vars));
		i++;
	}
	env->exit_code = EXIT_SUCCESS;
}

static int	check_flag(char *s)
{
	if (*(s++) == '-')
	{
		while (*s == 'n')
			s++;
		if (!*s)
			return (1);
	}
	return (0);
}

void	echo(t_cmd *cmd, t_env *env)
{
	int	flag;
	int	i;

	flag = 0;
	i = 1;
	if (cmd->argv_top >= 1)
		flag = check_flag(cmd->argv[1]);
	if (flag)
		i++;
	while (i <= cmd->argv_top)
	{
		printf("%s", cmd->argv[i]);
		if (i != cmd->argv_top)
			printf(" ");
		i++;
	}
	if (!flag)
		printf("\n");
	env->exit_code = EXIT_SUCCESS;
}

void	ft_exit(t_cmd *cmd, t_env *env)
{
	int	i;
	int	exit_code;

	i = 0;
	exit_code = 0;
	if (cmd->argv_top > 1)
		ft_putendl_fd("exit: too many args", STDERR_FILENO);
	if (cmd->argv_top == 1)
	{
		while (cmd->argv[1][i])
		{
			if (!ft_isdigit(cmd->argv[1][i]))
			{
				ft_putendl_fd("exit: use numeric code", STDERR_FILENO);
				break ;
			}
			i++;
		}
	}
	if (cmd->argv_top == 1 && !(cmd->argv[1][i]))
		exit_code = ft_atoi(cmd->argv[1]);
	free_env_vars(env->env_vars);
	free_env_vars(env->global_env_vars);
	free_parse_tree(env->parse_tree);
	clear_history();
	exit(exit_code);
}
