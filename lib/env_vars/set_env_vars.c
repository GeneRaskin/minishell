/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env_vars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:14:01 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 15:19:28 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/env_vars.h"
#include "../../include/env_state.h"
#include "../../include/libft.h"
#include "../../include/error.h"

extern t_env	*g_env;

t_env_vars	*init_env_vars(char *key, char *value, t_env *env);

static int	init(char *key, char *value,
				t_env_vars **vars, t_env_vars **vars_ptr)
{
	size_t		keylen;

	if (!*vars)
	{
		*vars = init_env_vars(key, value, g_env);
		return (0);
	}
	*vars_ptr = *vars;
	keylen = ft_strlen((*vars_ptr)->name);
	if (!ft_strncmp(key, (*vars_ptr)->name, keylen)
		&& keylen == ft_strlen(key))
	{
		free((*vars_ptr)->value);
		(*vars_ptr)->value = value;
		return (0);
	}
	return (1);
}

static void	update_vars_ptr(t_env_vars **vars_ptr, char *key,
							char *value)
{
	*vars_ptr = (*vars_ptr)->next;
	(*vars_ptr)->name = key;
	(*vars_ptr)->value = value;
	(*vars_ptr)->next = NULL;
}

void	set(char *key, char *value, t_env_vars **vars, t_env *env)
{
	t_env_vars	*vars_ptr;
	size_t		keylen;

	if (!init(key, value, vars, &vars_ptr))
		return ;
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
	update_vars_ptr(&vars_ptr, key, value);
}
