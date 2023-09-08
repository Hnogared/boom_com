#include "../Includes/defuser_wizard.h"

int	exec_command(portopts **conn_options, dispopts **disp_options)
{
	if (!left_strcmp("exit\n", (*disp_options)->cmd))
		exit_helper(*conn_options, *disp_options);
	if (!left_strcmp("man\n", (*disp_options)->cmd))
	{
		system("less defuser_man.txt");
		return (0);
	}
	check_view_cmds(disp_options);
	check_help_cmds(disp_options);
	check_choice(conn_options, disp_options);
	return (0);
}

void	print_output(portopts *conn_options, dispopts **disp_options)
{
	int		size;
	char	temp[BIG_BUFFER];

	if (conn_options->fd != -1)
	{
		size = read(conn_options->fd, temp, BIG_BUFFER - 1);
		if (size < 0)
		{
			strncpy((*disp_options)->bomb_output, "!> READING_ERROR >> ", BIG_BUFFER);
			strncpy((*disp_options)->bomb_output + 20, strerror(errno), BIG_BUFFER);
			(*disp_options)->bomb_output[BIG_BUFFER - 1] = 0;
			return ;
		}
		temp[size] = 0;
		if (strchr(temp, '$'))
			memmove((*disp_options)->bomb_output, temp, size + 1);
	}
	if ((*disp_options)->view == 1)
		return ;
	mvprintw(2, 0, "%s\n", crop((*disp_options)->bomb_output));
	if ((*disp_options)->layout == 3 && strstr((*disp_options)->bomb_output, "RECONFIGURATION"))
	{
		goto_layout_firewalloff(disp_options);
		menu_defusing(&conn_options, disp_options);
	}
	if ((*disp_options)->layout == 3 && strstr((*disp_options)->bomb_output, "firewall corrupted"))
	{
		goto_layout_firewalloff(disp_options);
		menu_defusing(&conn_options, disp_options);
	}
	if ((*disp_options)->prompt_char == '$' && strstr((*disp_options)->bomb_output, "SUPERUSER"))
		(*disp_options)->prompt_char = '#';
	if ((*disp_options)->view != 1 && conn_options->fd >= 0)
	{
		printw("\nUSER ~ %c ", (*disp_options)->prompt_char);
		if ((*disp_options)->cmd && (*disp_options)->cmd[0] == '@')
			printw("%s", (*disp_options)->cmd + 1);
		printw("\n");
	}
}

void	update_command(dispopts **disp_options, portopts **conn_options)
{
	int		ch;

	ch = get_keypress();
	if (ch == '\n' && (*disp_options)->cmd[0] == '@')
	{
		if ((*conn_options)->fd < 0)
			strncpy((*disp_options)->bomb_output, "!> WRITING ERROR >> Aucune connection etablie.", BIG_BUFFER);
		else if (write((*conn_options)->fd, (*disp_options)->cmd + 1, (*disp_options)->cmd_len - 1) == -1)
		{
			strncpy((*disp_options)->bomb_output, "!> WRITING ERROR >> ", BIG_BUFFER);
			strncpy((*disp_options)->bomb_output + 20, strerror(errno), BIG_BUFFER);
			(*disp_options)->bomb_output[BIG_BUFFER - 1] = 0;
		}
		bzero((*disp_options)->cmd, (*disp_options)->cmd_len + 1);
		(*disp_options)->cmd_len = 0;
		if ((*disp_options)->view == 1)
			(*disp_options)->view = 2;
	}
	else if (ch == '\n' && (*disp_options)->cmd_len && (*disp_options)->cmd_len < LITTLE_BUFFER - 2)
	{
		(*disp_options)->cmd[(*disp_options)->cmd_len] = ch;
		(*disp_options)->cmd[(*disp_options)->cmd_len + 1] = 0;
		exec_command(conn_options, disp_options);
		bzero((*disp_options)->cmd, (*disp_options)->cmd_len + 1);
		(*disp_options)->cmd_len = 0;
		if ((*disp_options)->view == 0)
			(*disp_options)->view = 2;
	}
	else if (ch == 127 && (*disp_options)->cmd_len > 0)
	{
		(*disp_options)->cmd[(*disp_options)->cmd_len - 1] = '\0';
		(*disp_options)->cmd_len--;
	}
	else if (ch == '\t')
		(*disp_options)->view = ((*disp_options)->view + 1) % 3;
	else if (((ch != ERR && ch > ' ' && ch != 127) || (ch == ' ' && (*disp_options)->cmd_len))
		&& (*disp_options)->cmd_len < LITTLE_BUFFER - 3)
	{
		(*disp_options)->cmd[(*disp_options)->cmd_len] = ch;
		(*disp_options)->cmd[(*disp_options)->cmd_len + 1] = 0;
		(*disp_options)->cmd_len++;
	}
}

void	print_prompt(portopts **conn_options, dispopts **disp_options)
{
	if ((*conn_options)->fd < 0)
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

	// Print command prompt
	attron(COLOR_PAIR(4));
	mvprintw(LINES - 2, 0, "%*s", COLS, "");
	printw("%*s", COLS, "");
	mvprintw(LINES - 1, 0, "(Command) $~ %s", (*disp_options)->cmd);
	move(LINES - 1, 13 + (*disp_options)->cmd_len);
	attroff(COLOR_PAIR(4));
}

void	menu_defusing(portopts **conn_options, dispopts **disp_options)
{
	clear();
	refresh();
	
	// Print the output of the bomb
	attron(A_BOLD);
	attron(COLOR_PAIR(2));
	mvprintw(0, 0, "%*s", COLS, "");
	if ((*disp_options)->view == 0)
		mvprintw(0, 0, "[1 BOMB INTERPRETOR][2 ...]");
	if ((*disp_options)->view == 1)
		mvprintw(0, 0, "[1 ...][2 DEFUSER GUI]");
	if ((*disp_options)->view == 2)
		mvprintw(0, 0, "[1 BOMB INTERPRETOR]");
	if ((*conn_options)->port[0])
		mvprintw(0, COLS - 35, "(PORT %-19.19s @ %06d)\n", (*conn_options)->port, get_baudrate((*conn_options)->baudrate));
	else
		mvprintw(0, COLS - 9, "(No port)\n");
	attroff(COLOR_PAIR(2));
	attroff(A_BOLD);
	if ((*disp_options)->bomb_output[0] == '!')
		attron(COLOR_PAIR(1));
	print_output(*conn_options, disp_options);
	if ((*disp_options)->bomb_output[0] == '!')
		attroff(COLOR_PAIR(1));
	// Print the debugger console and the output of the rpi
	if ((*disp_options)->view == 2)
	{
		attron(A_BOLD);
		attron(COLOR_PAIR(2));
		printw("                      [2 DEFUSER GUI]");
		printw("%*s", COLS - 37, "");
		attroff(COLOR_PAIR(2));
		attroff(A_BOLD);
		printw("\n");
	}
	if ((*disp_options)->view != 0)
	{
		if ((*disp_options)->cmd_output[0] == '!')
			attron(COLOR_PAIR(1));
		printw("%s\n", (*disp_options)->cmd_output);
		if ((*disp_options)->cmd_output[0] == '!')
			attroff(COLOR_PAIR(1));
	}
	print_prompt(conn_options, disp_options);
}
