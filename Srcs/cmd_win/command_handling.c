#include "../../Includes/defuser_wizard.h"

int	get_keypress(void)
{
	int	ch;

	halfdelay(1);
	ch = getch();
	while (ch == ERR)
		ch = getch();
	nocbreak();
	return (ch);
}

int	exec_command(t_data *data_p)
{
	if (!strcmp("exit", data_p->dispopts_s.cmd)
			|| !strcmp("1", data_p->dispopts_s.cmd))
		exit_helper(*data_p);
	if (!strcmp("man", data_p->dispopts_s.cmd))
	{
		system("less defuser_man.txt");
		return (0);
	}
	check_view_cmds(&data_p->dispopts_s);
	check_help_cmds(data_p);
	check_util_cmds(data_p);
	check_choice(data_p);
	check_conn_cmds(&data_p->portopts_s, &data_p->dispopts_s);
	return (0);
}

static void	forward_to_readline(char c, t_rlncurses *rlncurses_p)
{
	rlncurses_p->input = c;
	rlncurses_p->input_avail = true;
	rl_callback_read_char();
}

void	update_command(t_data *data_p)
{
	int	c;

	cbreak();
//	timeout(200);
	halfdelay(10);
	c = wgetch(data_p->dispopts_s.cmd_win);
//	fprintf(stderr, "%c\n", c);
	switch (c)
	{
	case ERR:
		read_bomb_out(data_p);
		break ;
	case KEY_RESIZE:
		resize(data_p);
		break ;
	case '\f':
		CHECK(clearok, curscr, TRUE);
		resize(data_p);
		break ;
	case '\t':
		data_p->dispopts_s.view = (data_p->dispopts_s.view + 1) % 3;
		main_layout(data_p);
		break ;
	default:
		forward_to_readline(c, &data_p->rlncurses_s);
	}
}
