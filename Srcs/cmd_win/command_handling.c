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
	check_help_cmds(&data_p->portopts_s, &data_p->dispopts_s);
	check_choice(&data_p->portopts_s, &data_p->dispopts_s);
	check_conn_cmds(&data_p->portopts_s, &data_p->dispopts_s);
	return (0);
}

static void	forward_to_readline(char c, t_rlncurses *rlncurses_p)
{
	rlncurses_p->input = c;
	rlncurses_p->input_avail = true;
	rl_callback_read_char();
}

void	update_command(t_portopts *portopts_p, t_dispopts *dispopts_p,
	t_rlncurses *rlncurses_p)
{
	int	c;

	cbreak();
	c = wgetch(dispopts_p->cmd_win);
	switch (c)
	{
	case KEY_RESIZE:
		resize(portopts_p, dispopts_p);
		break ;
	case '\f':
		CHECK(clearok, curscr, TRUE);
		resize(portopts_p, dispopts_p);
		break ;
	case '\t':
		dispopts_p->view = (dispopts_p->view + 1) % 3;
		main_layout(portopts_p, dispopts_p);
		break ;
	default:
		forward_to_readline(c, rlncurses_p);
	}
}
