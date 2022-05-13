/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 22:21:46 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 22:26:57 by lemmon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_private.h"
#include "../../include/libft.h"
#include "../../include/env_vars.h"

char		*substring(t_env *env);
void		word(t_env *env, t_curr_items_ptrs *ptrs);

void	word_main_loop(t_env *env, char **word_ptr)
{
	char	*temp;
	char	*joined;

	*word_ptr = substring(env);
	if (env->error_custom_msg || env->error_func_name)
		return ;
	while (match(SUBSTRING, env) || match(DOUBLE_QUOTE, env)
		|| match(SINGLE_QUOTE, env) || match(DOLLAR_SIGN, env))
	{
		temp = *word_ptr;
		*word_ptr = substring(env);
		if (env->error_custom_msg || env->error_func_name)
			return ;
		joined = ft_strjoin(temp, *word_ptr);
		if (!joined)
		{
			free(temp);
			free(*word_ptr);
			set_err_func_name(env, "malloc");
			return ;
		}
		free(*word_ptr);
		*word_ptr = joined;
		free(temp);
	}
}

void	set_inputs_outputs(t_env *env, t_cmd *curr_cmd)
{
	if (legal_lookahead(env, HEREDOC, APPEND_FILE,
			IN_FILE, OUT_FILE, NULL_TOKEN))
	{
		if (legal_lookahead(env, APPEND_FILE, OUT_FILE, NULL_TOKEN))
		{
			env->state |= OUT_FILE_TK;
			curr_cmd->out_filenames_top++;
		}
		if (match(APPEND_FILE, env)
			&& curr_cmd->out_filenames_top <= MAX_OUTFILENAMES - 1)
			curr_cmd->append_mode[curr_cmd->out_filenames_top]
				= 1;
		else if (match(IN_FILE, env))
			env->state |= IN_FILE_TK;
		else if (match(OUT_FILE, env)
			&& curr_cmd->out_filenames_top <= MAX_OUTFILENAMES - 1)
			curr_cmd->append_mode[curr_cmd->out_filenames_top]
				= 0;
		else if (match(HEREDOC, env))
			env->state |= HEREDOC_TK;
		advance(env, 1);
	}
}

void	start_cmd(t_cmd *curr_cmd, char *word, t_env *env, int flag)
{
	if (flag == 1)
	{	
		if (curr_cmd->in_filenames_top < MAX_INFILENAMES - 1)
			curr_cmd->in_filename[++curr_cmd->in_filenames_top] = word;
		else
			set_err_custom_msg(env, INFILENAMES_OVERFLOW_ERROR);
		curr_cmd->last_input = 1;
	}
	if (flag == 2)
	{	
		if (curr_cmd->out_filenames_top <= MAX_OUTFILENAMES - 1)
			curr_cmd->out_filename[curr_cmd->out_filenames_top] = word;
		else
		{
			curr_cmd->out_filenames_top--;
			set_err_custom_msg(env, OUTFILENAMES_OVERFLOW_ERROR);
		}
	}
}

void	check_env(t_env *env, t_cmd *curr_cmd, char *word)
{
	if (env->state & IN_FILE_TK)
		start_cmd(curr_cmd, word, env, 1);
	else if (env->state & OUT_FILE_TK)
		start_cmd(curr_cmd, word, env, 2);
	else if (env->state & HEREDOC_TK)
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
