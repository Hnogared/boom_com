#include "../../Includes/defuser_wizard.h"

static void	interpret_bomb_out(char *bomb_out, int size, t_data *data_p)
{
	memmove(data_p->dispopts_s.bomb_output, bomb_out, size + 1);
	if (data_p->dispopts_s.layout == 3 && strstr(bomb_out, "RECONFIGURATION"))
		goto_layout_firewalloff(data_p);
	if (data_p->dispopts_s.layout == 4 && strstr(bomb_out, "firewall corrupt"))
		goto_layout_labyrinth(data_p);
	if (data_p->dispopts_s.layout == 5 && strstr(bomb_out, "end_lab"))
		goto_layout_bytes(data_p);
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

static int	store_bomb_out(t_data *data_p)
{
	int		size;
	char	*trimmed;
	char	temp[BOMBOUT_BUFFER];

	size = read(data_p->portopts_s.fd, temp, BOMBOUT_BUFFER - 1);
	if (size <= 0)
		return (size);
	temp[size] = 0;
	trimmed = ft_strtrim(temp, "$ \t\n\v\f\r");
	if (!trimmed)
		return (-1);
	strncpy(data_p->dispopts_s.bomb_output, trimmed, size);
	free(trimmed);
	return (size);
}

void	read_bomb_out(t_data *data_p)
{
	int	size;

	if (!data_p || data_p->portopts_s.fd == -1)
	{
		if (!data_p->dispopts_s.bomb_output[0]
			|| data_p->dispopts_s.bomb_output[1] != '!')
			safer_strncpy(data_p->dispopts_s.bomb_output, " ! [ No device ] ",
				BOMBOUT_BUFFER, 18);
		return ;
	}
	size = store_bomb_out(data_p);
	if (size == -1)
	{
		save_error(data_p->dispopts_s.bomb_output, BOMBOUT_BUFFER, BIN_NAME,
			__func__);
		return ;
	}
	if (!size)
		return ;
	interpret_bomb_out(data_p->dispopts_s.bomb_output, size, data_p);
}

void	write_bomb_in(t_data *data_p, char *message)
{
	if (!data_p || data_p->portopts_s.fd == -1 || write(data_p->portopts_s.fd,
		message, strlen(message)) == -1)
	{
		if (!data_p->dispopts_s.bomb_output[0]
			|| data_p->dispopts_s.bomb_output[1] != '!')
			safer_strncpy(data_p->dispopts_s.bomb_output, " ! [ No device ] ",
				BOMBOUT_BUFFER, 18);
	}
}
