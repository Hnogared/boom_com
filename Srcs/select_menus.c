#include "../Includes/defuser_wizard.h"

char	*menu_port_select(void)
{
	int		i;
	int		ch;
	char	*port;
	char	**files_tab;

	ch = 0;
	files_tab = get_files_tab("/dev");
	port = NULL;
	while (1)
	{
		clear();
		refresh();
		
		put_separation(0, COLS);
		mvprintw(0, 0, "[PORT SELECTION MENU]");
		move(2, 0);
		if (files_tab)
		{
			i = -1;
			while (files_tab[++i])
				printw(" [%02d]   %s\n", i + 1, files_tab[i]);
			printw("\n [R]	Refresh\n");
			printw(" [ESC]  Cancel\n");
			if (ch)
				mvprintw(LINES - 1, 0, ">> [ERROR] Invalid input, please try again: ");
			else
				mvprintw(LINES - 2, 0, "\nPlease type one of the above (1-%d | r-R | ESC) to proceed: ", i);
		}
		else
			break ;
		ch = get_keypress();
		if (ch >= '1' && ch <= '0' + i)
		{
			port = files_tab[ch - '0' - 1];
			free(files_tab);
			return(port);
		}
		if (ch == 'r' || ch == 'R')
		{
			ch = 0;
			free(files_tab);
			files_tab = get_files_tab("/dev");
		}
		if (ch == '\e')
		{
			free(files_tab);
			break ;
		}
	}
	return(NULL);
}

int	menu_baudrate_select(void)
{
	int	ch;

	ch = 0;
	while (1)
	{
		clear();
		refresh();

		put_separation(0, COLS);
		mvprintw(0, 0, "[BAUD RATE SELECTION MENU]");
		move(2, 0);
		printw("This is where you select the connection speed between *CONFEDERATION BOMB DEFUSER v4.6*\nand the bomb.\n");
		printw("The data/commands sent to the bomb have to be synchronized to its speed for it to be able\nto interpret them correctly.\n");
		printw("Please be aware that an incorrect synchronisation speed may send corrupted data/commands tothe bomb and trigger its defense mechanisms.\n\n");
		printw("For more informations on the topic of synchronizing communication speeds with a lethal bombplease read the fucking manual.\n\n");
		printw(" [01]   9600\n");
		printw(" [02]   42000\n");
		printw(" [03]   115200\n");
		printw("\n [ESC]  Cancel\n");
		if (ch)
			mvprintw(LINES - 1, 0, ">> [ERROR] Invalid input, please try again: ");
		else
			mvprintw(LINES - 2, 0, "\nPlease type one of the above (1-3 | ESC) to proceed: ");
		ch = get_keypress();
		if (ch >= '1' && ch <= '3')
			return (ch - '0');
		if (ch == 27)
			break ;
	}
	return(0);
}
