#include "../Includes/defuser_wizard.h"

int	check_view_cmds(char *command, dispopts **disp_options)
{
	if (!left_strcmp("set-view bomb\n", command))
	{
		(*disp_options)->view = 0;
		return (1);
	}
	else if (!left_strcmp("set-view defuser\n", command))
	{
		(*disp_options)->view = 1;
		return (1);
	}
	else if (!left_strcmp("set-view split\n", command))
	{
		(*disp_options)->view = 2;
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
			(*conn_options)->port = ft_strjoin("/dev/", temp);
		if ((*conn_options)->port && (*conn_options)->fd > -1 && cfgetispeed((*conn_options)->toptions))
			play_connect(conn_options);
		return (1);
	}
	else if (!left_strcmp("set-baudrate\n", (*disp_options)->cmd))
	{
		if (!(*conn_options)->port && (*disp_options)->view)
		{
			(*disp_options)->cmd_output = "ERROR >> Please select a port before setting up the baud rate.";
			return (0);
		}
		baudrate = menu_baudrate_select();
		if (baudrate)
		{
			if ((*conn_options)->fd <= -1)
				play_connect(conn_options);
			else
				*(*conn_options)->toptions = set_termios_opt((*conn_options)->fd, (*conn_options)->baudrate);
		}
		return (1);
	}
	return (0);
}

int	check_help_cmds(char *command, dispopts **disp_options)
{
	if (!left_strcmp("help\n", command) && (*disp_options)->view)
	{
		(*disp_options)->cmd_output = "List of the available important commands :\n\n\
For more information, type 'help_cmd' or refer to the manual (cmd 'man').\n\n\
# help\t\tShow this menu.\n\
# man\t\tOpen the manual.\n\
# exit\t\tClose *CONFEDERATION BOMB DEFUSER v4.6*.\n\
# help cmd\tQuick guide on how to send commands through the defuser.\n\
# help nav\tDisplay the commands for navigating through the defuser.\n\
# help connect\tQuick guide on how to connect to a bomb.\n\
# help hacks\tQuick guide on hacking attacks on electronics/informatics.\n";
		return (1);
	}
	else if (!left_strcmp("help cmd\n", command) && (*disp_options)->view)
	{
		printw("The *CONFEDERATION BOMB DEFUSER v4.6* is an interface for using disarming tools on\nterrorist devices.\n\n");
		printw("There are two types of commands that can be sent through the program :\n");
		printw(" * Direct commands that will be interpreted by the defuser itself.\n");
		printw(" * Commands starting with '@' that will be sent to the bomb to interact with it.\n\n");
		printw("Please be aware that the Confederation cannot be taken as responsible for the outcome of\nany command sent to a bomb.\n\n");
		printw("You can type 'help' or read the manual (cmd 'man') for more information.\n");
		return (1);
	}
	else if (!left_strcmp("help nav\n", command) && (*disp_options)->view)
	{
		printw("It is possible to navigate and rearrange the workspace as needed.\n");
		printw("This can be achieved by using the following commands.\n\n");
		printw("NOTE : You can quickly cycle between workspace arrangements by pressing 'TAB'.\n\n");
		printw("List of commands to navigate through the GUI of the defuser :\n\n");
		printw("# exit\t\t\tClose *CONFEDERATION BOMB DEFUSER v4.6*.\n");
		printw("# set-view bomb\t\tSwitch to bomb view only.\n");
		printw("# set-view difuser\tSwitch to defuser view only.\n");
		printw("# set-view split\tSplit the screen horizontally between the bomb and defuser views.\n");
		return (1);
	}
	else if (!left_strcmp("help connect\n", command) && (*disp_options)->view)
	{
		printw("Quick guide on how to setup a connection between your defuser and a bomb.\n\n");
		printw("1. Connect one of the defuser's USB ports to the device to disarm.\n");
		printw("2. Run 'set_port' and select an output USB port.\n");
		printw("3. Following the port, use 'set_baudrate' to select a baud rate.\n");
		printw("4. You should then get a connection confirmation in the 'BOMB INTERPRETOR' tab.\n");
		printw("Depending on the bomb type, you might get an audio/visual confirmation from it as well.\n\n");
		printw("ISSUES:\n");
		printw("* If connecting doesn't seem to have succeeded, check your port and/or baud rate settings.\n");
		printw("* A connection through the correct USB port will link with the bomb.\n");
		printw("However, an invalid baud rate will lead to communication issues with the device.\n");
		printw("If this seems to be the case for you, try out different rates through the same port.\n\n");
		printw("More information on connecting to a bomb in the manual (cmd 'man').\n");
		printw("# set-port\tChange the defuser output USB port.\n");
		printw("# set-baudrate\tChange the port data transfer rate.\n");
		return (1);
	}
	else if (!left_strcmp("help hacks\n", command) && (*disp_options)->view)
	{
		printw("Quick guide on hacking attacks on electronics/informatics usign the defuser.\n\n");
		printw("1. Look for vulnerabilities : outdated software, open ports.\n");
		printw("2. Select one of the tools we provide to exploit a potential vulnerability.\n\n");
		printw("Available hacking programs:\n");
		printw("# firewall-ext [ID]\tProgram to overload a component's firewall.\n");
		printw("# bytestuffer [ID]\tMake a given component's memory overflow.\n");
		printw("More information on hacking into electronic/informatic systems in the manual (cmd 'man').\n");
		return (1);
	}
	return (0);
}
