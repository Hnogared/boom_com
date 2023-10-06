#include "../../Includes/defuser_wizard.h"

t_history	*new_history_node(char *cmd, char *bomb_out, void *previous,
	void *next)
{
	t_history	*new_node;

	new_node = (t_history *) malloc(sizeof(t_history));
	if (!new_node)
		return (NULL);
	new_node->cmd = cmd;
	new_node->bomb_output = bomb_out;
	new_node->previous = previous;
	new_node->next = next;
	return (new_node);
}

t_history	*history_add_front(t_history **history_p, t_history *new_node_p)
{
	if (!history_p || !new_node_p)
		return (NULL);
	new_node_p->next = *history_p;
	if (*history_p)
		(*history_p)->previous = new_node_p;
	*history_p = new_node_p;
	return (*history_p);
}

void	free_history_node(t_history *node_p)
{
	if (!node_p)
		return ;
	if (node_p->cmd)
		free(node_p->cmd);
	if (node_p->bomb_output)
		free(node_p->bomb_output);
	free(node_p);
}

void	free_history(t_history *history_p)
{
	t_history *next_p;

	while (history_p)
	{
		next_p = history_p->next;
		free_history_node(history_p);
		history_p = next_p;
	}
}
