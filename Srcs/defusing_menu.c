#include "../Includes/defuser_wizard.h"

int	exec_command(portopts **conn_options, dispopts **disp_options)
{
	if (!left_strcmp("exit\n", (*disp_options)->cmd))
		exit_helper(*conn_options, *disp_options);
	if (!left_strcmp("man\n", (*disp_options)->cmd))
	{
		system("less defuser_man.txt");
		return (1);
	}
	if (check_help_cmds(disp_options))
		return (0);
	if (check_view_cmds(disp_options) || check_conn_cmds(conn_options, disp_options))
		return (1);
	if ((*disp_options)->view)
	{
		strncpy((*disp_options)->cmd_output, "ERROR >> Unknown command\n"
			"Type 'help' for a list of all defusing assistant commands or get the manual (cmd 'man').", BIG_BUFFER);
		(*disp_options)->cmd_output[BIG_BUFFER - 1] = 0;
	}
	return (0);
}

void	print_output(portopts *conn_options, dispopts **disp_options)
{

	if (conn_options->fd < 0)
		return ;
	bzero((*disp_options)->bomb_output, BIG_BUFFER);
	if (read(conn_options->fd, (*disp_options)->bomb_output , BIG_BUFFER - 1) < 0)
	{
		strncpy((*disp_options)->cmd_output, "ERROR >> Unable to read the device's output", BIG_BUFFER);
		return ;
	}
	if ((*disp_options)->view == 1)
		return ;
	mvprintw(2, 0, "%s\n", crop((*disp_options)->bomb_output));
	if ((*disp_options)->prompt_char == '$' && strstr((*disp_options)->bomb_output, "SUPERUSER"))
		(*disp_options)->prompt_char = '#';
	if ((*disp_options)->view != 1 && conn_options->fd >= 0)
	{
		printw("\nUSER ~ %c ", (*disp_options)->prompt_char);
		if ((*disp_options)->cmd && (*disp_options)->cmd[0] == '@')
			printw((*disp_options)->cmd + 1);
		printw("\n");
	}
}

void	update_command(dispopts **disp_options, portopts **conn_options)
{
	int		ch;

	ch = get_keypress();
	
	if (ch == '\n' && (*disp_options)->cmd[0] == '@')
	{
		if (write((*conn_options)->fd, (*disp_options)->cmd + 1, (*disp_options)->cmd_len - 1) == -1)
		{
			endwin();
			perror("write");
			exit(1);
		}
		memset((*disp_options)->cmd, 0, (*disp_options)->cmd_len);
		(*disp_options)->cmd_len = 0;
		if ((*disp_options)->view == 1)
			(*disp_options)->view = 2;
	}
	else if (ch == '\n' && (*disp_options)->cmd_len < LITTLE_BUFFER - 2)
	{
		(*disp_options)->cmd[(*disp_options)->cmd_len] = ch;
		(*disp_options)->cmd[(*disp_options)->cmd_len + 1] = 0;
		exec_command(conn_options, disp_options);
		memset((*disp_options)->cmd, 0, (*disp_options)->cmd_len);
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
		&& (*disp_options)->cmd_len < LITTLE_BUFFER - 2)
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
		attron(COLOR_PAIR(3));
		printw("/!\\ Currently not connected to any device. /!\\\n");
		printw("Type 'help_connect' or read the manual (cmd 'man') for how to connect to a bomb.");
		attroff(COLOR_PAIR(3));
	}
	// Print command prompt
	put_separation(LINES - 2, COLS);
	printw("Command: %-300s", (*disp_options)->cmd);
	move(LINES - 1, 9 + (*disp_options)->cmd_len);
	
	update_command(disp_options, conn_options);
}

int	menu_defusing(portopts **conn_options, dispopts **disp_options)
{
	while (1)
	{
		clear();
		refresh();
		
		// Print the output of the bomb
		attron(COLOR_PAIR(2));
		put_separation(0, COLS);
		if ((*disp_options)->view == 0)
			mvprintw(0, 0, "[1 BOMB INTERPRETOR][2 ...]");
		if ((*disp_options)->view == 1)
			mvprintw(0, 0, "[1 ...][2 DEFUSER GUI]");
		if ((*disp_options)->view == 2)
			mvprintw(0, 0, "[1 BOMB INTERPRETOR]");
		if ((*conn_options)->port[0])
			mvprintw(0, COLS - 35, "(PORT %-19.19s @ %06d)\n", (*conn_options)->port, get_baudrate((*conn_options)->toptions));
		else
			mvprintw(0, COLS - 9, "(No port)\n");
		print_output(*conn_options, disp_options);
		attroff(COLOR_PAIR(2));

		// Print the debugger console and the output of the rpi
		if ((*disp_options)->view == 2)
		{
			attron(COLOR_PAIR(2));
			printw("______________________[2 DEFUSER GUI]");
			put_separation(-1, COLS - 37);
			attroff(COLOR_PAIR(2));
			printw("\n");
		}
		printw("%s\n", (*disp_options)->cmd_output);
		print_prompt(conn_options, disp_options);
	}
	return (0);
}
