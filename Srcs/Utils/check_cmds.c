#include "../Includes/defuser_wizard.h"

int	check_view_cmds(dispopts **disp_options)
{
	if (!left_strcmp("set-view bomb\n", (*disp_options)->cmd))
		(*disp_options)->view = 0;
	else if (!left_strcmp("set-view defuser\n", (*disp_options)->cmd))
		(*disp_options)->view = 1;
	else if (!left_strcmp("set-view split\n", (*disp_options)->cmd))
		(*disp_options)->view = 2;
	else
		return (0);
	return (1);
}

int	check_choice(portopts **conn_options, dispopts **disp_options)
{
	if (((*disp_options)->cmd)[0] == '1' && ((*disp_options)->cmd)[1] == '\n')
		exit_helper(*conn_options, *disp_options);
	if (((*disp_options)->cmd)[0] == '2' && ((*disp_options)->cmd)[1] == '\n')
	{
		if ((*disp_options)->layout == -2 || (*disp_options)->layout == 1)
			goto_layout_2(conn_options, disp_options, false);
		else if ((*disp_options)->layout == -3)
			goto_layout_3(disp_options, false);
		else
			goto_layout_1(conn_options, disp_options);
		return (1);
	}
	if (((*disp_options)->cmd)[0] == '3' && ((*disp_options)->cmd)[1] == '\n')
	{
		if ((*disp_options)->layout == 2)
			goto_layout_3(disp_options, true);
		return (1);
	}
	return (0);
}

int	check_conn_cmds(portopts **conn_options, dispopts **disp_options)	
{
	int		baudrate;
	char	*temp;

	if (!left_strcmp("set-port\n", (*disp_options)->cmd))
	{
		temp = menu_port_select();
		if (temp)
		{
			memmove((*conn_options)->port, "/dev/", 5);
			memmove((*conn_options)->port + 5, temp, strlen(temp));
			(*conn_options)->port[5 + strlen(temp)] = 0;
		}
		if ((*conn_options)->port && (*conn_options)->fd > -1 && cfgetispeed((*conn_options)->toptions))
			play_connect(conn_options, disp_options);
		return (1);
	}
	else if (!left_strcmp("set-baudrate\n", (*disp_options)->cmd))
	{
		if (!*(*conn_options)->port && (*disp_options)->view)
		{
			strncpy((*disp_options)->cmd_output, "!> ERROR >> Please select a port before setting up the baud rate.", BIG_BUFFER);
			(*disp_options)->cmd_output[BIG_BUFFER - 1] = 0;
			return (1);
		}
		baudrate = menu_baudrate_select();
		if (baudrate)
		{
			if ((*conn_options)->fd <= -1)
				play_connect(conn_options, disp_options);
			else
				set_termios_opt((*conn_options)->fd, (*conn_options)->baudrate);
		}
		return (1);
	}
	return (0);
}

int	check_help_cmds(dispopts **disp_options)
{
	if (!left_strcmp("help\n", (*disp_options)->cmd) && (*disp_options)->view)
		goto_layout_help(disp_options);
	else if (!left_strcmp("help cmd\n", (*disp_options)->cmd) && (*disp_options)->view)
	{
		strncpy((*disp_options)->cmd_output, "$> The *CONFEDERATION BOMB DEFUSER v4.6* is an interface for using disarming tools on terrorist devices.\n\n"
			"   There are two types of commands that can be sent through the program :\n"
			"    * Direct commands that will be interpreted by the defuser itself.\n"
			"    * Commands starting with '@' that will be sent to the bomb to interact with it.\n\n"
			"   Please be aware that the Confederation cannot be taken as responsible for the outcome of\nany command sent to a bomb.\n\n"
			"   You can type 'help' or read the manual (cmd 'man') for more information.\n", BIG_BUFFER);
		(*disp_options)->cmd_output[BIG_BUFFER - 1] = 0;
	}
	else if (!left_strcmp("help nav\n", (*disp_options)->cmd) && (*disp_options)->view)
	{
		strncpy((*disp_options)->cmd_output, "$> It is possible to navigate and rearrange the workspace as needed.\n"
			"   This can be achieved by using the following commands.\n\n"
			"   NOTE : You can quickly cycle between workspace arrangements by pressing 'TAB'.\n\n"
			"   List of commands to navigate through the GUI of the defuser :\n\n"
			"   # exit              Close *CONFEDERATION BOMB DEFUSER v4.6*.\n"
			"   # set-view bomb     Switch to bomb view only.\n"
			"   # set-view defuser  Switch to defuser view only.\n"
			"   # set-view split    Split the screen horizontally between the bomb and defuser views.\n", BIG_BUFFER);
		(*disp_options)->cmd_output[BIG_BUFFER - 1] = 0;
	}
	else if (!left_strcmp("help connect\n", (*disp_options)->cmd) && (*disp_options)->view)
	{
		strncpy((*disp_options)->cmd_output, "$> Quick guide on how to setup a connection between your defuser and a bomb.\n\n"
			"   1. Connect one of the defuser's USB ports to the device to disarm.\n"
			"   2. Run 'set-port' and select an output USB port.\n"
			"   3. Following the port, use 'set-baudrate' to select a baud rate.\n"
			"   4. You should then get a connection confirmation in the 'BOMB INTERPRETOR' tab.\n"
			"   Depending on the bomb type, you might get an audio/visual confirmation from it as well.\n\n"
			"   ISSUES:\n"
			"   * If connecting doesn't seem to have succeeded, check your port and/or baud rate settings.\n"
			"   * A connection through the correct USB port will link with the bomb.\n"
			"   However, an invalid baud rate will lead to communication issues with the device.\n"
			"   If this seems to be the case for you, try out different rates through the same port.\n\n"
			"   More information on connecting to a bomb in the manual (cmd 'man').\n"
			"   # set-port      Change the defuser output USB port.\n"
			"   # set-baudrate  Change the port data transfer rate.\n", BIG_BUFFER);
		(*disp_options)->cmd_output[BIG_BUFFER - 1] = 0;
	}
	else if (!left_strcmp("help hacks\n", (*disp_options)->cmd) && (*disp_options)->view)
	{
		strncpy((*disp_options)->cmd_output, "$> Quick guide on hacking attacks on electronics/informatics usign the defuser.\n\n"
			"   1. Look for vulnerabilities : outdated software, open ports.\n"
			"   2. Select one of the tools we provide to exploit a potential vulnerability.\n\n"
			"   Available hacking programs:\n"
			"   # firewall-ext [ID]  Program to overload a component's firewall.\n"
			"   # bytestuffer [ID]   Make a given component's memory overflow.\n"
			"   More information on hacking into electronic/informatic systems in the manual (cmd 'man').\n", BIG_BUFFER);
		(*disp_options)->cmd_output[BIG_BUFFER - 1] = 0;
	}
	else
		return (0);
	return (1);
}
