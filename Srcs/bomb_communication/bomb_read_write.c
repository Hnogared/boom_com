#include "../../Includes/defuser_wizard.h"

void	read_bomb_out(t_data *data_p)
{
	int		size;
	char	temp[BIG_BUFFER];

	if (data_p->portopts_s.fd == -1)
		return ;
	size = read(data_p->portopts_s.fd, temp, BIG_BUFFER - 1);
	if (size < 0)
	{
		save_error(data_p->dispopts_s.bomb_output, BOMBOUT_BUFFER, BIN_NAME,
			"read error");
		return ;
	}
	temp[size] = 0;
	if (strchr(temp, '$'))
		memmove(data_p->dispopts_s.bomb_output, temp, size + 1);
	if (data_p->dispopts_s.layout == 3 && strstr(temp, "RECONFIGURATION"))
		goto_layout_firewalloff(data_p);
	if (data_p->dispopts_s.layout == 4 && strstr(temp, "firewall corrupted"))
		goto_layout_labyrinth(data_p);
//	if (dispopts_p->layout == 5 && strstr(dispopts_p->bomb_output, "end_lab"))
//		goto_layout_bytes(&conn_options, disp_options);
	if (data_p->dispopts_s.prompt_char == '$' && strstr(temp, "SUPERUSER"))
		data_p->dispopts_s.prompt_char = '#';
	if (data_p->dispopts_s.view != DEFUSER_VIEW && data_p->portopts_s.fd != -1)
	{
		printw("\nUSER ~ %c ", data_p->dispopts_s.prompt_char);
		if (data_p->dispopts_s.cmd && data_p->dispopts_s.cmd[0] == '@')
			printw("%s", data_p->dispopts_s.cmd + 1);
		printw("\n");
	}
}