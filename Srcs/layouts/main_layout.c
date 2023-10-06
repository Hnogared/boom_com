#include "../Includes/defuser_wizard.h"

void	read_bomb_out(t_portopts *portopts_p, t_dispopts *dispopts_p)
{
	int		size;
	char	temp[BIG_BUFFER];

	if (portopts_p->fd < -1)
		return ;
	size = read(portopts_p->fd, temp, BIG_BUFFER - 1);
	if (size < 0)
	{
		save_error(dispopts_p->bomb_output, BIG_BUFFER, BIN_NAME, "read error");
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
	if (dispopts_p->view != 1 && portopts_p->fd >= 0)
	{
		printw("\nUSER ~ %c ", dispopts_p->prompt_char);
		if (dispopts_p->cmd && dispopts_p->cmd[0] == '@')
			printw("%s", dispopts_p->cmd + 1);
		printw("\n");
	}
}

static void	print_tabs(t_portopts portopts_s, t_dispopts dispopts_s)
{
	attron(A_BOLD);
	attron(COLOR_PAIR(2));
	mvprintw(0, 0, "%*s", COLS, "");
	if (dispopts_s.view == 0)
		mvprintw(0, 0, "[1 BOMB INTERPRETOR][2 ...]");
	if (dispopts_s.view == 1)
		mvprintw(0, 0, "[1 ...][2 DEFUSER GUI]");
	if (dispopts_s.view == 2)
		mvprintw(0, 0, "[1 BOMB INTERPRETOR]");
	if (portopts_s.port[0])
		mvprintw(0, COLS - 35, "(PORT %-19.19s @ %06d)\n", portopts_s.port,
			get_baudrate(portopts_s.baudrate));
	else
		mvprintw(0, COLS - 9, "(No port)\n");
	attroff(COLOR_PAIR(2));
	attroff(A_BOLD);
}

static void	print_outputs(t_dispopts dispopts_s)
{
	if (dispopts_s.view != DEFUSER_VIEW)
	{
		if (dispopts_s.bomb_output[0] == '!')
			attron(COLOR_PAIR(1));
		mvprintw(2, 0, "%s\n", crop(dispopts_s.bomb_output));
		if (dispopts_s.bomb_output[0] == '!')
			attroff(COLOR_PAIR(1));
	}
	if (dispopts_s.view == BOMB_VIEW)
		return ;
	if (dispopts_s.view == SPLIT_VIEW)
	{
		attron(A_BOLD);
		attron(COLOR_PAIR(2));
		printw("                      [2 DEFUSER GUI]%*s", COLS - 37, "");
		attroff(COLOR_PAIR(2));
		attroff(A_BOLD);
		printw("\n");
	}
	if (dispopts_s.cmd_output[0] == '!')
		attron(COLOR_PAIR(1));
	printw("%s\n", dispopts_s.cmd_output);
	if (dispopts_s.cmd_output[0] == '!')
		attroff(COLOR_PAIR(1));
}

static void	print_conn_error(t_portopts *portopts_p)
{
	if (portopts_p->fd < 0)
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

void	menu_defusing(t_portopts *portopts_p, t_dispopts *dispopts_p)
{
	CHECK(werase, dispopts_p->win);
	read_bomb_out(portopts_p, dispopts_p);
	print_tabs(*portopts_p, *dispopts_p);
	print_outputs(*dispopts_p);
	print_conn_error(portopts_p);
	attron(COLOR_PAIR(4));
	mvprintw(LINES - 1, 0, "%s", PROMPT " ");
	attroff(COLOR_PAIR(4));
	CHECK(refresh);
}
