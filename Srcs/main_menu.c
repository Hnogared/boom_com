#include "../Includes/defuser_wizard.h"

char	g_prompt = '$';

int	exec_command(portopts *conn_options, char *command, int *view)
{
	if (!left_strcmp("exit\n", command))
		exit_helper(conn_options->fd, command);
	if (!left_strcmp("man\n", command))
	{
		system("less defuser_man.txt");
		return (1);
	}
	if (check_help_cmds(command, view))
		return (0);
	if (check_view_cmds(command, view) || check_conn_cmds(conn_options, command, view))
		return (1);
	if (*view)
	{
		printw("ERROR >> Unknown command : %s\n", command);
		printw("Type 'help' for a list of all defusing assistant commands or get the manual (cmd 'man').");
	}
	return (0);
}

char	*print_output(int fd, char *last_out, int view)
{
	char	buf[255];
	char	*buf2;
	char	*temp;

	if (!fd)
		return (NULL);
	memset(buf, 0, sizeof(buf));
	buf2 = NULL;
	if (read(fd, buf, sizeof(buf)) > 0)
	{
		temp = buf2;
		buf2 = ft_strjoin(buf2, buf);
		free(temp);
	}
	if (view == 1)
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

char	*print_prompt(portopts *conn_options, char *curr_cmd, char *last_cmd, int *view)
{
	int		ch;
	char	*cmd;
	
	if (curr_cmd)
		cmd = strdup(curr_cmd);
	else
		cmd = (char *) calloc(255, sizeof(char));
	if (last_cmd)
	{
		if (exec_command(conn_options, last_cmd, view))
			return (NULL);
	}
	if (conn_options->fd < 0)
	{
		mvprintw(LINES - 5, 0, "___________________________________________________________________________________________");
		printw("/!\\ Currently not connected to any device. /!\\\n");
		printw("Type 'help_connect' or read the manual (cmd 'man') for how to connect to a bomb.");
	}
	// Print command prompt
	put_separation(LINES - 2);
	printw("Command: %-0300s", cmd);
	move(LINES - 1, 9 + strlen(cmd));
	
	halfdelay(1);
	ch = getch();
	nocbreak();
	
	if (ch == '\n' && cmd[0] == '@')
	{
		if (write(conn_options->fd, cmd + 1, strlen(cmd + 1)) == -1)
		{
			endwin();
			perror("write");
			exit(1);
		}
		memset(cmd, 0, strlen(cmd));
		if (*view == 1)
			*view = 2;
	}
	else if (ch == '\n' && strlen(cmd) < 253)
	{
		cmd[strlen(cmd)] = ch;
		if (*view == 0)
			*view = 2;
	}
	else if (ch == 127 && strlen(cmd) > 0)
		cmd[strlen(cmd) - 1] = '\0';
	else if (ch == '\t')
		*view = (*view + 1) % 3;
	else if (((ch != ERR && ch > ' ' && ch != 127) || (ch == ' ' && strlen(cmd)))
		&& strlen(cmd) < 253)
		cmd[strlen(cmd)] = ch;
	return (cmd);
}

int	menu_defusing(portopts *conn_options)
{
	int		view;
	char	*cmd = NULL;
	char	*temp_cmd;
	char	*last_cmd = NULL;
	char	*out = NULL;
	char	*temp_out = NULL;

	view = 1;
	while (1) {
		clear();
		refresh();
		
		// Print the output of the bomb
		if (view == 0)
			mvprintw(0, 0, "[1 BOMB INTERPRETOR][2 ...]_________________________________(PORT %-15.15s @ %06d)", g_port, get_baudrate(conn_options->toptions));
		if (view == 2)
			mvprintw(0, 0, "[1 BOMB INTERPRETOR]________________________________________(PORT %-15.15s @ %06d)", g_port, get_baudrate(conn_options->toptions));
		if (out)
		{
			temp_out = crop(out);
			if (strstr(out, "SUPERUSER"))
				g_prompt = '#';
		}
		out = print_output(conn_options->fd, temp_out, view);
		if ((view == 0 || view == 2) && conn_options->fd >= 0)
		{
			printw("\nUSER ~ %c ", g_prompt);
			if (cmd && cmd[0] == '@')
				printw(cmd + 1);
			printw("\n");
		}
		
		// Print the debugger console and the output of the rpi
		if (view == 1)
			mvprintw(0, 0, "[1 ...][2 DEFUSER GUI]______________________________________(PORT %-15.15s @ %06d)\n", g_port, get_baudrate(conn_options->toptions));
		if (view == 2)
			printw("______________________[2 DEFUSER GUI]______________________________________________________\n");
		temp_cmd = cmd;
		cmd = print_prompt(conn_options, cmd, last_cmd, &view);
		if (temp_cmd)
			free(temp_cmd);
		if (!cmd || (cmd && cmd[strlen(cmd) - 1] == '\n'))
		{
			if (last_cmd)
			{
				free(last_cmd);
				last_cmd = NULL;
			}
			if (cmd)
			{
				last_cmd = strdup(cmd);
				memset(cmd, 0, strlen(cmd));
			}
		}
	}
	return (0);
}
