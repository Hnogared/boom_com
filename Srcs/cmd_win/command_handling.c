#include "../../Includes/defuser_wizard.h"

int	exec_command(t_portopts *portopts_p, t_dispopts *dispopts_p)
{
	if (!strcmp("exit", dispopts_p->cmd))
		exit_helper(*portopts_p, *dispopts_p);
	if (!strcmp("man", dispopts_p->cmd))
	{
		system("less defuser_man.txt");
		return (0);
	}
	check_view_cmds(dispopts_p);
	check_help_cmds(portopts_p, dispopts_p);
	check_choice(portopts_p, dispopts_p);
	check_conn_cmds(portopts_p, dispopts_p);
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
