#include "../../include/env_vars.h"
#include "../../include/env_state.h"
#include "../../include/libft.h"
#include "../../include/error.h"

void	free_env_var(t_env_vars *env_var)
{
	free(env_var->value);
	free(env_var->name);
	free(env_var);
}

char	*get(char *key, t_env_vars *vars)
{
	size_t	keylen;

	while (vars)
	{
		keylen = ft_strlen(vars->name);
		if (ft_strncmp(key, vars->name, keylen) == 0
			&& keylen == ft_strlen(key))
			return (vars->value);
		vars = vars->next;
	}
	return (NULL);
}

static t_env_vars	*init_env_vars(char *key, char *value, t_env *env)
{
	t_env_vars	*env_vars;

	env_vars = (t_env_vars *) malloc(sizeof(t_env_vars));
	if (!env_vars)
	{
		env->error_func_name = "malloc";
		return (NULL);
	}
	env_vars->next = NULL;
	env_vars->value = value;
	env_vars->name = key;
	return (env_vars);
}

void	free_env_vars(t_env_vars *vars)
{
	t_env_vars	*curr;

	curr = vars;
	while (curr)
	{
		vars = vars->next;
		free_env_var(curr);
		curr = vars;
	}
}

void	unset(char *key, t_env_vars **env_vars)
{
	t_env_vars	*prev;
	t_env_vars	*curr;

	curr = *env_vars;
	if (curr != NULL && !ft_strncmp(key, curr->name, ft_strlen(curr->name))
		&& ft_strlen(curr->name) == ft_strlen(key))
	{
		*env_vars = curr->next;
		free_env_var(curr);
		return ;
	}
	while (curr != NULL && (ft_strncmp(key, curr->name, ft_strlen(curr->name))
			|| ft_strlen(curr->name) != ft_strlen(key)))
	{
		prev = curr;
		curr = curr->next;
	}
	if (curr == NULL)
		return ;
	prev->next = curr->next;
	free_env_var(curr);
}

void	set(char *key, char *value, t_env_vars **vars, t_env *env)
{
	t_env_vars	*vars_ptr;
	size_t		keylen;

	if (!*vars)
	{
		*vars = init_env_vars(key, value, env);
		return ;
	}
	vars_ptr = *vars;
	keylen = ft_strlen(vars_ptr->name);
	if (!ft_strncmp(key, vars_ptr->name, keylen)
		&& keylen == ft_strlen(key))
	{
		free(vars_ptr->value);
		vars_ptr->value = value;
		return ;
	}
	while (vars_ptr->next)
	{
		vars_ptr = vars_ptr->next;
		keylen = ft_strlen(vars_ptr->name);
		if (!ft_strncmp(key, vars_ptr->name, keylen)
			&& keylen == ft_strlen(key))
		{
			free(vars_ptr->value);
			vars_ptr->value = value;
			return ;
		}
	}
	vars_ptr->next = (t_env_vars *) malloc(sizeof(t_env_vars));
	if (!vars_ptr->next)
	{
		env->error_func_name = "malloc";
		return ;
	}
	vars_ptr = vars_ptr->next;
	vars_ptr->name = key;
	vars_ptr->value = value;
	vars_ptr->next = NULL;
}
