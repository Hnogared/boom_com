#include "../Includes/defuser_wizard.h"

int	exec_command(t_portopts *portopts_p, t_dispopts *dispopts_p)
{
	if (!left_strcmp("exit\n", dispopts_p->cmd))
		exit_helper(*portopts_p, *dispopts_p);
	if (!left_strcmp("man\n", dispopts_p->cmd))
	{
		system("less defuser_man.txt");
		return (0);
	}
	check_view_cmds(dispopts_p);
	check_help_cmds(g_data_s.dispopts_s.msg_win, portopts_p, dispopts_p);
	check_choice(g_data_s.dispopts_s.msg_win, portopts_p, dispopts_p);
	return (0);
}

void	print_output(WINDOW *win, t_portopts *portopts_p, t_dispopts *dispopts_p)
{
	int		size;
	char	temp[BIG_BUFFER];

	if (portopts_p->fd != -1)
	{
		size = read(portopts_p->fd, temp, BIG_BUFFER - 1);
		if (size < 0)
		{
			strncpy(dispopts_p->bomb_output, "!> READING_ERROR >> ", BIG_BUFFER);
			strncpy(dispopts_p->bomb_output + 20, strerror(errno), BIG_BUFFER);
			dispopts_p->bomb_output[BIG_BUFFER - 1] = 0;
			return ;
		}
		temp[size] = 0;
		if (strchr(temp, '$'))
			memmove(dispopts_p->bomb_output, temp, size + 1);
	}
	if (dispopts_p->view == 1)
		return ;
	mvwprintw(win, 2, 0, "%s\n", crop(dispopts_p->bomb_output));
	if (dispopts_p->layout == 3 && strstr(dispopts_p->bomb_output, "RECONFIGURATION"))
		goto_layout_firewalloff(win, portopts_p, dispopts_p);
	if (dispopts_p->layout == 4 && strstr(dispopts_p->bomb_output, "firewall corrupted"))
		goto_layout_labyrinth(win, portopts_p, dispopts_p);
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

void cmd_win_redisplay(WINDOW *cmd_win, bool for_resize)
{
	size_t prompt_width = strwidth(rl_display_prompt, 0);
	size_t cursor_col = prompt_width +
						strnwidth(rl_line_buffer, rl_point, prompt_width);

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

void	update_command(t_portopts *portopts_p, t_dispopts *dispopts_p)
{
	char	*line;
//	char	c;

	cbreak();
	line = readline(NULL);
//	move(LINES - 2, 0);
//	get_keypress(NULL);
	rl_callback_read_char();	
	if (portopts_p || dispopts_p)
		return ;
	fprintf(stderr, "%s\n", line);
}

void	print_prompt(t_portopts *portopts_p, t_dispopts *dispopts_p)
{

	// Print command prompt
	attron(COLOR_PAIR(4));
	mvprintw(LINES - 2, 0, "%*s", COLS, "");
//	printw("%*s", COLS, "");
//	mvprintw(LINES - 1, 0, "(Command) $~ %s", dispopts_p->cmd);
//	move(LINES - 1, 13 + dispopts_p->cmd_len);
//	move(LINES - 1, 0);
	attroff(COLOR_PAIR(4));
	if (portopts_p || dispopts_p)
		return ;
}

void	menu_defusing(WINDOW *msg_win, t_portopts *portopts_p, t_dispopts *dispopts_p)
{
	CHECK(werase, msg_win);

	attron(A_BOLD);
	attron(COLOR_PAIR(2));
	mvwprintw(msg_win, 0, 0, "%*s", COLS, "");
	if (dispopts_p->view == 0)
		mvwprintw(msg_win, 0, 0, "[1 BOMB INTERPRETOR][2 ...]");
	if (dispopts_p->view == 1)
		mvwprintw(msg_win, 0, 0, "[1 ...][2 DEFUSER GUI]");
	if (dispopts_p->view == 2)
		mvwprintw(msg_win, 0, 0, "[1 BOMB INTERPRETOR]");
	if (portopts_p->port[0])
		mvwprintw(msg_win, 0, COLS - 35, "(PORT %-19.19s @ %06d)\n", portopts_p->port, get_baudrate(portopts_p->baudrate));
	else
		mvwprintw(msg_win, 0, COLS - 9, "(No port)\n");
	attroff(COLOR_PAIR(2));
	attroff(A_BOLD);
	// Print the output of the bomb
	if (dispopts_p->bomb_output[0] == '!')
		attron(COLOR_PAIR(1));
	print_output(msg_win, portopts_p, dispopts_p);
	if (dispopts_p->bomb_output[0] == '!')
		attroff(COLOR_PAIR(1));
	// Print the debugger console and the output of the rpi
	if (dispopts_p->view == 2)
	{
		attron(A_BOLD);
		attron(COLOR_PAIR(2));
		printw("                      [2 DEFUSER GUI]");
		printw("%*s", COLS - 37, "");
		attroff(COLOR_PAIR(2));
		attroff(A_BOLD);
		printw("\n");
	}
	if (dispopts_p->view != 0)
	{
		if (dispopts_p->cmd_output[0] == '!')
			attron(COLOR_PAIR(1));
		printw("%s\n", dispopts_p->cmd_output);
		if (dispopts_p->cmd_output[0] == '!')
			attroff(COLOR_PAIR(1));
	}

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
}
