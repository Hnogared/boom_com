#include "../Includes/defuser_wizard.h"

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

void	read_bomb_out(t_portopts *portopts_p, t_dispopts *dispopts_p)
{
	int		size;
	char	temp[BIG_BUFFER];

	if (portopts_p->fd < -1)
		return ;
	size = read(portopts_p->fd, temp, BIG_BUFFER - 1);
	if (size < 0)
	{
		strcpy(dispopts_p->bomb_output, "!> READING_ERROR >> ");
		strcpy(dispopts_p->bomb_output + 20, strerror(errno));
		dispopts_p->bomb_output[BIG_BUFFER - 1] = 0;
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

void	print_bomb_out(t_dispopts dispopts_s)
{
	if (dispopts_s.view == 1)
		return ;
	if (dispopts_s.bomb_output[0] == '!')
		attron(COLOR_PAIR(1));
	mvprintw(2, 0, "%s\n", crop(dispopts_s.bomb_output));
	if (dispopts_s.bomb_output[0] == '!')
		attroff(COLOR_PAIR(1));
}

void cmd_win_redisplay(WINDOW *cmd_win, bool for_resize)
{
	size_t prompt_width;
	size_t cursor_col;

	prompt_width = strwidth(rl_display_prompt, 0);
	cursor_col = prompt_width + strnwidth(rl_line_buffer, rl_point, prompt_width);
	CHECK(werase, cmd_win);
	// This might write a string wider than the terminal currently, so don't
	// check for errors
	mvwprintw(cmd_win, 0, 0, "%s%s", rl_display_prompt, rl_line_buffer);
	if (cursor_col >= (size_t) COLS)
		// Hide the cursor if it lies outside the window. Otherwise it'll
		// appear on the very right.
		curs_set(0);
	else {
		CHECK(wmove, cmd_win, 0, cursor_col);
		curs_set(2);
	}
	// We batch window updates when resizing
	if (for_resize)
		CHECK(wnoutrefresh, cmd_win);
	else
		CHECK(wrefresh, cmd_win);
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
		menu_defusing(portopts_p, dispopts_p);
		break ;
	default:
		forward_to_readline(c, rlncurses_p);
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
		mvprintw(0, COLS - 35, "(PORT %-19.19s @ %06d)\n", portopts_s.port, get_baudrate(portopts_s.baudrate));
	else
		mvprintw(0, COLS - 9, "(No port)\n");
	attroff(COLOR_PAIR(2));
	attroff(A_BOLD);
}

static void	print_prompt(t_portopts *portopts_p)
{
	if (portopts_p->fd < 0)
	{
		put_separation(LINES - 5, COLS);
		attron(A_BOLD);
		attron(COLOR_PAIR(3));
		put_centered("/!\\ Aucune connection etablie. /!\\", -1, COLS);
		printw("%*s", COLS, "");
		put_centered("Veuillez verifier la connection USB et reessayez.", LINES - 3, COLS);
		attroff(A_BOLD);
		attroff(COLOR_PAIR(3));
	}
	else
		put_separation(LINES - 3, COLS);
	attron(COLOR_PAIR(4));
	mvprintw(LINES - 2, 0, "%*s", COLS, "");
	printw("%*s", COLS, "");
	mvprintw(LINES - 1, 0, "%s", PROMPT " ");
	attroff(COLOR_PAIR(4));
}

static void	print_cmd_out(t_dispopts dispopts_s)
{
	if (dispopts_s.view == 2)
	{
		attron(A_BOLD);
		attron(COLOR_PAIR(2));
		printw("                      [2 DEFUSER GUI]");
		printw("%*s", COLS - 37, "");
		attroff(COLOR_PAIR(2));
		attroff(A_BOLD);
		printw("\n");
	}
	if (dispopts_s.view != 0)
	{
		if (dispopts_s.cmd_output[0] == '!')
			attron(COLOR_PAIR(1));
		printw("%s\n", dispopts_s.cmd_output);
		if (dispopts_s.cmd_output[0] == '!')
			attroff(COLOR_PAIR(1));
	}

}

void	menu_defusing(t_portopts *portopts_p, t_dispopts *dispopts_p)
{
	CHECK(werase, dispopts_p->win);
	read_bomb_out(portopts_p, dispopts_p);
	print_tabs(*portopts_p, *dispopts_p);
	// Print the output of the bomb
	print_bomb_out(*dispopts_p);
	// Print the debugger console and the output of the rpi
	print_cmd_out(*dispopts_p);
	print_prompt(portopts_p);
	CHECK(refresh);
}
