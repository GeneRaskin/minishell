#include "../../include/libft.h"
#include <stdio.h>
#include "../../include/env_state.h"
#include "../../include/error.h"
#include "../../include/parse_tree.h"
#include "../../include/env_vars.h"
#include "../../include/free.h"

void	cd(t_cmd *cmd, t_env *env)
{
	char	*pwd_value;

	if (cmd->argv_top != 1)
	{
		env->error_custom_msg = CD_ARGC_ERROR;
		return ;
	}
	if (chdir(cmd->argv[1]) == -1)
	{
		env->error_custom_msg = CD_PATH_ERROR;
		return ;
	}
	pwd_value = ft_strjoin("PWD=", getcwd(NULL, ft_strlen(cmd->argv[1]) + 255));
	set("PWD", pwd_value, &(env->env_vars), env);
}

void	pwd(void)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	ft_putstr_fd(buf, STDOUT_FILENO);
	write(STDOUT_FILENO, "\n", 1);
	free(buf);
}

void	ft_env(t_env_vars *vars)
{
	while (vars != NULL)
	{
		printf("%s=%s\n", vars->name, vars->value);
		vars = vars->next;
	}
}

static int	ft_check_flag(char *s)
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

void	echo(char **s)
{
	int	flag;

	flag = 0;
	while (*(++s))
	{
		if (!ft_check_flag(*s))
		{
			ft_putstr_fd(*s, 1);
			if (*(s + 1) && !ft_check_flag(*(s + 1)))
				write(1, " ", 1);
		}
		else
			flag = 1;
	}
	if (flag == 0)
		write(1, "\n", 1);
}

void	ft_exit(t_env *env)
{
	free_env_vars(env->env_vars);
	//free_parse_tree(parse_tree);
	exit(EXIT_SUCCESS);
}
