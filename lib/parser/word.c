/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 22:19:23 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 22:26:47 by lemmon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_private.h"
#include "../../include/libft.h"
#include "../../include/env_vars.h"

char		*substring(t_env *env);
void		word(t_env *env, t_curr_items_ptrs *ptrs);
void		check_env(t_env *env, t_cmd *curr_cmd, char *word);
void		start_cmd(t_cmd *curr_cmd, char *word, t_env *env, int flag);
void		set_inputs_outputs(t_env *env, t_cmd *curr_cmd);
void		word_main_loop(t_env *env, char **word_ptr);

static int	verify_var_name(t_env *env)
{
	char	*var;
	int		len;

	len = 0;
	var = env->yytext;
	if (!ft_isalpha(*var))
		return (0);
	while (ft_isalnum(*var))
	{
		var++;
		len++;
	}
	if (*var == '+' && *(var + 1) == '=')
	{
		env->yyleng = len;
		env->state |= APPEND_TO_VAR;
		return (1);
	}
	if (*var == '=')
	{
		env->yyleng = len;
		return (1);
	}
	return (0);
}

static void	init_env_var(t_env *env)
{
	char	*key;
	char	*word;
	char	*temp;
	int		in_global_vars;

	key = ft_substr(env->yytext, 0, env->yyleng);
	in_global_vars = 0;
	if (!key)
	{
		env->error_func_name = "malloc";
		return ;
	}
	env->state |= PARSE_VAR;
	advance(env, 1);
	if (match(PLUS_SIGN, env))
		advance(env, 1);
	advance(env, 1);
	word_main_loop(env, &word);
	if (env->error_custom_msg || env->error_func_name)
		return ;
	if (get(key, env->global_env_vars))
		in_global_vars = 1;
	if (env->state & APPEND_TO_VAR)
	{
		env->state &= ~APPEND_TO_VAR;
		temp = word;
		if (!in_global_vars)
		{
			word = get(key, env->env_vars);
			if (word)
			{
				word = ft_strjoin(word, temp);
				free(temp);
			}
			else
				word = temp;
		}
		else
		{
			word = get(key, env->global_env_vars);
			if (word)
			{
				word = ft_strjoin(word, temp);
				free(temp);
			}
			else
				word = temp;
		}
	}
	if (!in_global_vars)
		set(key, word, &(env->env_vars), env);
	else
		set(key, word, &(env->global_env_vars), env);
	env->state &= ~PARSE_VAR;
}

void	word(t_env *env, t_curr_items_ptrs *ptrs)
{
	char		*word;
	t_cmd		*curr_cmd;

	curr_cmd = ptrs->curr_pipelst->u_item.cmd;
	if (curr_cmd->argv_top == -1 && verify_var_name(env))
	{
		init_env_var(env);
		return ;
	}
	set_inputs_outputs(env, curr_cmd);
	word_main_loop(env, &word);
	check_env(env, curr_cmd, word);
	curr_cmd->argv[curr_cmd->argv_top + 1] = NULL;
	env->state &= ~IN_FILE_TK;
	env->state &= ~OUT_FILE_TK;
	env->state &= ~HEREDOC_TK;
}
