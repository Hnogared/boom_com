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
		mvprintw(0, 0, "YOOOOOO %s\n", (*disp_options)->cmd);
		strncpy((*disp_options)->cmd_output, "ERROR >> Unknown command\n"
			"Type 'help' for a list of all defusing assistant commands or get the manual (cmd 'man').", BIG_BUFFER);
		(*disp_options)->cmd_output[BIG_BUFFER - 1] = 0;
	}
	return (0);
}

char	*print_output(portopts *conn_options, char *last_out, dispopts **disp_options)
{
	char	buf[BIG_BUFFER + 1];
	char	*buf2;
	char	*temp;

	if (conn_options->fd < 0)
		return (NULL);
	memset(buf, 0, sizeof(buf));
	buf2 = NULL;
	if (read(conn_options->fd, buf, BIG_BUFFER) > 0)
	{
		temp = buf2;
		buf2 = ft_strjoin(buf2, buf);
		free(temp);
	}
	if ((*disp_options)->view == 1)
		return (last_out);
	if (buf2 && buf2[strlen(buf2) - 3] == '$')
	{
		mvprintw(2, 0, "%s\n", crop(buf2));
		return (buf2);
	}
	if (last_out)
		mvprintw(2, 0, "%s\n", last_out);
	return (last_out);
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
		printw("/!\\ Currently not connected to any device. /!\\\n");
		printw("Type 'help_connect' or read the manual (cmd 'man') for how to connect to a bomb.");
	}
	// Print command prompt
	put_separation(LINES - 2, COLS);
	printw("Command: %-300s", (*disp_options)->cmd);
	move(LINES - 1, 9 + (*disp_options)->cmd_len);
	
	update_command(disp_options, conn_options);
}

int	menu_defusing(portopts **conn_options, dispopts **disp_options)
{
	char	*out = NULL;
	char	*temp_out = NULL;

	while (1) {
		clear();
		refresh();
		
		// Print the output of the bomb
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
		if (out && (*disp_options)->prompt_char == '$')
		{
			temp_out = crop(out);
			if (strstr(out, "SUPERUSER"))
				(*disp_options)->prompt_char = '#';
		}
		out = print_output(*conn_options, temp_out, disp_options);
		if (((*disp_options)->view == 0 || (*disp_options)->view == 2) && (*conn_options)->fd >= 0)
		{
			printw("\nUSER ~ %c ", (*disp_options)->prompt_char);
			if ((*disp_options)->cmd && (*disp_options)->cmd[0] == '@')
				printw((*disp_options)->cmd + 1);
			printw("\n");
		}
		
		// Print the debugger console and the output of the rpi
		if ((*disp_options)->view == 2)
		{
			printw("______________________[2 DEFUSER GUI]");
			put_separation(-1, COLS - 37);
			printw("\n");
		}
		if ((*disp_options)->cmd_output && (*disp_options)->view > 0)
		{
			printw("%s\n", (*disp_options)->cmd_output);
		}
		print_prompt(conn_options, disp_options);
	}
	return (0);
}
