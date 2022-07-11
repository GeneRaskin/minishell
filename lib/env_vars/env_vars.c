/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:14:10 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 15:18:08 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

t_env_vars	*init_env_vars(char *key, char *value, t_env *env)
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
