#include "../../include/aux.h"

void	free_2d_arr(void **arr)
{
	int	i;

	i = -1;
	while (arr[++i] != NULL)
		free(arr[i]);
	free(arr);
}

static void	free_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i <= cmd->argv_top)
	{
		free(cmd->argv[i]);
		i++;
	}
	i = 0;
	while (i <= cmd->heredocs_top)
	{
		free(cmd->delimeters[i]);
		i++;
	}
	if (cmd->out_filename)
		free(cmd->out_filename);
	if (cmd->in_filename)
		free(cmd->in_filename);
	free(cmd);
}

static void	free_pipelist(t_pipelist *pipelist)
{
	t_pipelist	*curr;

	curr = pipelist;
	while (curr != NULL)
	{
		pipelist = pipelist->next;
		if (curr->type == NEXT_SCRIPT)
			free_parse_tree(curr->u_item.script);
		else if (curr->type == NEXT_PIPELST)
			free_cmd(curr->u_item.cmd);
		free(curr);
		curr = pipelist;
	}
}

static void	free_cmd_table(t_cmd_table *cmd_table)
{
	t_cmd_table	*curr;

	curr = cmd_table;
	while (curr != NULL)
	{
		cmd_table = cmd_table->next;
		free_pipelist(curr->pipelist);
		free(curr);
		curr = cmd_table;
	}
}

void	free_parse_tree(t_scripts *parse_tree)
{
	t_scripts	*curr;

	curr = parse_tree;
	while (curr != NULL)
	{
		parse_tree = parse_tree->next;
		free_cmd_table(curr->cmd_table);
		free(curr);
		curr = parse_tree;
	}
}
