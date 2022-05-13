/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 22:09:04 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 22:15:02 by lemmon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/env_vars.h"
#include "../../include/env_state.h"
#include "../../include/libft.h"
#include "../../include/error.h"

void	free_env_var(t_env_vars *env_var);
char	*get(char *key, t_env_vars *vars);
void	free_env_vars(t_env_vars *vars);
void	unset(char *key, t_env_vars **env_vars);

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

static void	ptr(t_env_vars *vars_ptr, size_t keylen, char *key, char *value)
{	
	while (vars_ptr->next)
	{
		vars_ptr = vars_ptr->next;
		keylen = ft_strlen(vars_ptr->name);
		if (!ft_strncmp(key, vars_ptr->name, keylen)
			&& keylen == ft_strlen(key))
		{
			free(vars_ptr->value);
			vars_ptr->value = value;
		}
	}
}	

static void	ptr2(t_env_vars	*vars_ptr, char *key, char *value)
{	
	vars_ptr = vars_ptr->next;
	vars_ptr->name = key;
	vars_ptr->value = value;
	vars_ptr->next = NULL;
}

static int	check_vars(t_env_vars **vars, char *key, char *value, t_env *env)
{
	if (!*vars)
	{
		*vars = init_env_vars(key, value, env);
		return (1);
	}
	return (0);
}

void	set(char *key, char *value, t_env_vars **vars, t_env *env)
{
	t_env_vars	*vars_ptr;
	size_t		keylen;

	if (check_vars(vars, key, value, env))
		return ;
	vars_ptr = *vars;
	keylen = ft_strlen(vars_ptr->name);
	if (!ft_strncmp(key, vars_ptr->name, keylen)
		&& keylen == ft_strlen(key))
	{
		free(vars_ptr->value);
		vars_ptr->value = value;
		return ;
	}
	ptr(vars_ptr, keylen, key, value);
	vars_ptr->next = (t_env_vars *) malloc(sizeof(t_env_vars));
	if (!vars_ptr->next)
	{
		env->error_func_name = "malloc";
		return ;
	}
	ptr2(vars_ptr, key, value);
}
