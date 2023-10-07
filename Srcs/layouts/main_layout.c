#include "../Includes/defuser_wizard.h"

static void	print_tabs(t_portopts portopts_s, t_dispopts dispopts_s)
{
	attron(COLOR_PAIR(2));
	mvprintw(0, 0, "%*s", COLS, "");
	if (portopts_s.port[0])
		mvprintw(0, COLS - 35, "(PORT %-19.19s @ %06d)", portopts_s.port,
			get_baudrate(portopts_s.baudrate));
	else
		mvprintw(0, COLS - 9, "(No port)\n");
	if (dispopts_s.view == BOMB_VIEW)
		attron(COLOR_PAIR(4));
	mvprintw(0, 1, " BOMB INTERPRETOR ");
	if (dispopts_s.view == BOMB_VIEW)
		attron(COLOR_PAIR(2));
	if (dispopts_s.view != SPLIT_VIEW)
	{
		if (dispopts_s.view == DEFUSER_VIEW)
			attron(COLOR_PAIR(4));
		printw(" DEFUSER GUIDE ");
		if (dispopts_s.view == DEFUSER_VIEW)
			attron(COLOR_PAIR(2));
	}
	move(2, 0);
	attroff(COLOR_PAIR(2));
}

static void	print_outputs(t_portopts portopts_s, t_dispopts dispopts_s)
{
	if (dispopts_s.view != DEFUSER_VIEW)
	{
		if (portopts_s.fd == -1 && dispopts_s.bomb_output[0])
			attron(COLOR_PAIR(1));
		mvprintw(2, 0, "%s\n\n", dispopts_s.bomb_output);
		if (portopts_s.fd == 1 && dispopts_s.bomb_output[0])
			attroff(COLOR_PAIR(1));
	}
	if (dispopts_s.view == BOMB_VIEW)
		return ;
	if (dispopts_s.view == SPLIT_VIEW)
	{
		attron(COLOR_PAIR(2));
		printw("                    DEFUSER GUIDE%*s\n", COLS - 33, "");
		attroff(COLOR_PAIR(2));
	}
	printw("%s\n", dispopts_s.cmd_output);
}

static void	print_conn_error(t_portopts portopts_s)
{
	if (portopts_s.fd < 0)
	{
		put_separation(LINES - 4, COLS);
		attron(A_BOLD);
		attron(COLOR_PAIR(3));
		put_centered("/!\\ Aucune connection etablie. /!\\", -1, COLS);
		printw("%*s", COLS, "");
		put_centered("Veuillez verifier la connection USB et reessayez.",
			LINES - 2, COLS);
		attroff(A_BOLD);
		attroff(COLOR_PAIR(3));
	}
	else
		put_separation(LINES - 2, COLS);
}

void	main_layout(t_data *data_p)
{
	CHECK(werase, data_p->dispopts_s.win);
	read_bomb_out(data_p);
	print_tabs(data_p->portopts_s, data_p->dispopts_s);
	print_outputs(data_p->portopts_s, data_p->dispopts_s);
	print_conn_error(data_p->portopts_s);
	attron(COLOR_PAIR(4));
	mvprintw(LINES - 1, 0, "%s", PROMPT " ");
	attroff(COLOR_PAIR(4));
	CHECK(refresh);
}
