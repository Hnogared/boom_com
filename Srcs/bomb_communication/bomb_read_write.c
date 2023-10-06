#include "../../Includes/defuser_wizard.h"

void	read_bomb_out(t_portopts *portopts_p, t_dispopts *dispopts_p)
{
	int		size;
	char	temp[BOMBOUT_BUFFER];

	if (portopts_p->fd == -1)
		return ;
	size = read(portopts_p->fd, temp, BIG_BUFFER - 1);
	if (size < 0)
	{
		save_error(dispopts_p->bomb_output, BOMBOUT_BUFFER, BIN_NAME, "read error");
		return ;
	}
	temp[size] = 0;
	if (strchr(temp, '$'))
		memmove(dispopts_p->bomb_output, temp, size + 1);
	if (dispopts_p->layout == 3 && strstr(dispopts_p->bomb_output, "RECONFIGURATION"))
		goto_layout_firewalloff(portopts_p, dispopts_p);
	if (dispopts_p->layout == 4 && strstr(dispopts_p->bomb_output, "firewall corrupted"))
		goto_layout_labyrinth(portopts_p, dispopts_p);
//	if (dispopts_p->layout == 5 && strstr(dispopts_p->bomb_output, "end_lab"))
//		goto_layout_bytes(&conn_options, disp_options);
	if (dispopts_p->prompt_char == '$' && strstr(dispopts_p->bomb_output, "SUPERUSER"))
		dispopts_p->prompt_char = '#';
	if (dispopts_p->view != DEFUSER_VIEW && portopts_p->fd >= 0)
	{
		printw("\nUSER ~ %c ", dispopts_p->prompt_char);
		if (dispopts_p->cmd && dispopts_p->cmd[0] == '@')
			printw("%s", dispopts_p->cmd + 1);
		printw("\n");
	}
}
