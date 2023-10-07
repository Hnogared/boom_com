#include "../../Includes/defuser_wizard.h"

static void	interpret_bomb_out(char *bomb_out, int size, t_data *data_p)
{
	memmove(data_p->dispopts_s.bomb_output, bomb_out, size + 1);
	if (data_p->dispopts_s.layout == 3 && strstr(bomb_out, "RECONFIGURATION"))
		goto_layout_firewalloff(data_p);
	if (data_p->dispopts_s.layout == 4 && strstr(bomb_out, "firewall corrupted"))
		goto_layout_labyrinth(data_p);
//	if (dispopts_p->layout == 5 && strstr(dispopts_p->bomb_output, "end_lab"))
//		goto_layout_bytes(&conn_options, disp_options);
	if (data_p->dispopts_s.prompt_char == '$' && strstr(bomb_out, "SUPERUSER"))
		data_p->dispopts_s.prompt_char = '#';
	if (data_p->dispopts_s.view != DEFUSER_VIEW && data_p->portopts_s.fd != -1)
	{
		printw("\nUSER ~ %c ", data_p->dispopts_s.prompt_char);
		if (data_p->dispopts_s.cmd && data_p->dispopts_s.cmd[0] == '@')
			printw("%s", data_p->dispopts_s.cmd + 1);
		printw("\n");
	}
}

void	read_bomb_out(t_data *data_p)
{
	int		size;
	char	*trimmed;
	char	temp[BIG_BUFFER];

	if (data_p->portopts_s.fd == -1)
	{
		if (!data_p->dispopts_s.bomb_output[0]
			|| data_p->dispopts_s.bomb_output[1] != '!')
			strncpy(data_p->dispopts_s.bomb_output, " ! [ No device ] ",
				BOMBOUT_BUFFER);
		return ;
	}
	size = read(data_p->portopts_s.fd, temp, BIG_BUFFER - 1);
	if (size < 0)
	{
		save_error(data_p->dispopts_s.bomb_output, BOMBOUT_BUFFER, BIN_NAME,
			"read error");
		goto_layout_1(data_p);
		return ;
	}
	if (!size)
		return ;
	temp[size] = 0;
	trimmed = ft_strtrim(temp, "$ \t\n\v\f\r");
	if (!trimmed)
	{
		save_error(data_p->dispopts_s.bomb_output, BOMBOUT_BUFFER, BIN_NAME,
			__func__);
		return ;
	}
	interpret_bomb_out(trimmed, size, data_p);
	free(trimmed);
}
