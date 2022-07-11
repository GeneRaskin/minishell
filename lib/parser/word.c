/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 02:51:55 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/14 02:51:57 by eugeneras        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_private.h"

static void	init_env_var(t_env *env)
{
	char	*key;
	char	*word;
	int		in_global_vars;

	if (!init_key_vars(&in_global_vars, &key, env))
		return ;
	word_main_loop(env, &word);
	if (env->error_custom_msg || env->error_func_name)
		return ;
	if (get(key, env->global_env_vars))
		in_global_vars = 1;
	if (env->state & APPEND_TO_VAR)
		append_to_word(env, in_global_vars, &word, key);
	if (!in_global_vars)
		set(key, word, &(env->env_vars), env);
	else
		set(key, word, &(env->global_env_vars), env);
	env->state &= ~PARSE_VAR;
}

static void	heredoc_case(t_env *env, t_cmd *curr_cmd,
							char *word)
{
	if (env->state & HEREDOC_TK)
	{
		if (curr_cmd->heredocs_top < MAX_HEREDOCS - 1)
			curr_cmd->delimeters[++curr_cmd->heredocs_top] = word;
		else
			set_err_custom_msg(env, HEREDOCS_OVERFLOW_ERROR);
		curr_cmd->last_input = 2;
	}
	else
	{
		if (curr_cmd->argv_top < MAX_ARGV - 2)
			curr_cmd->argv[++curr_cmd->argv_top] = word;
		else
			set_err_custom_msg(env, ARGV_OVERFLOW_ERROR);
	}
}

static void	check_overflows(t_env *env, t_cmd *curr_cmd, char *word)
{
	if (env->state & IN_FILE_TK)
	{
		if (curr_cmd->in_filenames_top < MAX_INFILENAMES - 1)
			curr_cmd->in_filename[++curr_cmd->in_filenames_top] = word;
		else
			set_err_custom_msg(env, INFILENAMES_OVERFLOW_ERROR);
		curr_cmd->last_input = 1;
	}
	else if (env->state & OUT_FILE_TK)
	{
		if (curr_cmd->out_filenames_top <= MAX_OUTFILENAMES - 1)
			curr_cmd->out_filename[curr_cmd->out_filenames_top] = word;
		else
		{
			curr_cmd->out_filenames_top--;
			set_err_custom_msg(env, OUTFILENAMES_OVERFLOW_ERROR);
		}
	}
	else
		heredoc_case(env, curr_cmd, word);
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
	check_overflows(env, curr_cmd, word);
	curr_cmd->argv[curr_cmd->argv_top + 1] = NULL;
	env->state &= ~IN_FILE_TK;
	env->state &= ~OUT_FILE_TK;
	env->state &= ~HEREDOC_TK;
}
