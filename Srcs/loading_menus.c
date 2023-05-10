#include "../Includes/defuser_wizard.h"

int	play_startup(void)
{
	int		i;
	int		ch;
	char	*padding;
	char	*loading_bar;
	
	padding = "----------------------";
	loading_bar = "############################################################";
	
	clear();
	
	mvprintw(0, 0, "Booting system");
	halfdelay(5);
	getch();
	nocbreak();
	
	clear();
	
	cbreak();
	timeout(100);
		
	i = 0;
	while(i < 20)
	{
		mvprintw(0, 0, "%s ======[%.*s", padding, i * 3, "CONFEDERATION BOMB DEFUSER v4.6");
		printw("]====== %s", padding);
		getch();
		i++;
	}
	
	timeout(50);
	
	i = 0;
	while (i <= 60)
	{
		mvprintw(3, 0, "LOADING SCRIPTS...");
		if (i == 60)
			printw(" (OK)");
		printw("\n[%d%%][%.*s]", i * 100 / 60, i, loading_bar);
		getch();
		i++;
	}
	
	i = 0;
	while (i <= 30)
	{
		mvprintw(6, 0, "CHECKING FOR UPDATES...");
		if (i == 30)
			printw(" (Up to date)");
		printw("\n[%d%%][%.*s]", i * 100 / 30, i * 2, loading_bar);
		getch();
		i++;
	}
	
	i = 0;
	while (i <= 30)
	{
		mvprintw(9, 0, "LOADING INTERFACE...");
		if (i >= 20)
			printw(" (OK)");
		printw("\n[%d%%][%.*s]", i * 3 * 100 / 30 > 100 ? 100 : i * 3 * 100 / 30, i * 3, loading_bar);
		mvprintw(12, 0, "LOADING MANUAL...");
		if (i == 30)
			printw(" (OK)");
		printw("\n[%d%%][%.*s]", i * 100 / 30, i * 2, loading_bar);
		getch();
		i++;
	}
	
	timeout(100);
	
	i = 0;
	while (i <= 4)
	{
		mvprintw(15, 0, "CONFIGURING PORTS...");
		if (i == 4)
			printw(" (OK)");
		if (i >= 1)
			printw("\n [01] Open");
		if (i >= 2)
			printw("\n [02] Open");
		if (i >= 3)
			printw("\n [03] Open");
		if (i == 4)
			printw("\n [04] Open");
		getch();
		i++;
	}
	
	mvprintw(LINES - 3, 0, "System operational.");
	mvprintw(LINES - 1, 0, "I have read and agree to the terms of service (press space to confirm) ");
	
	nocbreak();
	if (get_keypress() != ' ')
	{
		mvprintw(LINES - 1, 0, "Are you sure you don't want to agree to the terms of service and exit ? (y/N) ");
		ch = get_keypress();
		if (ch == 'y' || ch == 'Y')
			return (1);
	}
	return (0);
}

int play_connect(char *port_name, int baudrate, struct termios *toptions)
{
	int		i;
	int		fd;
	int		delay;
	char	*loading_bar;
	char	*separation;
	
	i = 0;
	clear();
	loading_bar = "########################################";
	separation = "___________________________________________________________________________________________";
	while (i <= 60)
	{
		mvprintw(0, 0, "[BOMB DEFUSER] Establishing connection...\n\n");
		printw("Please DO NOT at any time shut down the defuser during this process.");

		// Open serial port
		if (i == 55)
		{
			fd = open(port_name, O_RDWR | O_NOCTTY);
			if (fd == -1) {
				endwin();
				perror("open");
				printf("%s\n", port_name);
				return (-1);
			}
			*toptions = set_termios_opt(fd, baudrate);
		}
		
		printw("\n%s\n\n\n", separation);
		printw("Opening port...");
		if (i >= 20)
			printw(" (OK)");
		printw("\nLOADING [%d%%][%.*s]\n", (i * 100 / 20) > 100 ? 100 : (i * 100 / 20), i * 2, loading_bar);
		
		if (i >= 20)
		{
			printw("\n\n\nDecoding encryption...");
			if (i >= 40)
				printw(" (OK)");
			printw("\nLOADING [%d%%][%.*s]\n", ((i - 20) * 100 / 20) > 100 ? 100 : ((i - 20) * 100 / 20), (i - 20) * 2, loading_bar);
		}
		
		if (i >= 40)
		{
			printw("\n\n\nAnalyzing firmware...");
			if (i == 60)
				printw(" (OK)");
			printw("\nLOADING [%d%%][%.*s]\n", (i - 40) * 100 / 20, (i - 40) * 2, loading_bar);
		}
		
		mvprintw(LINES - 4, 0, "%s\n", separation);
		printw("PROGRESS [%d%%][%.*s]\n", i * 100 / 60, i, "############################################################");
		
		if (i == 60)
			printw("Connecting to device.");
		
		delay = 100;
		if (i < 20 || i > 40)
			delay = 50;
		if (i == 60)
			delay = 150;
		
		cbreak();
		timeout(delay);
		getch();
		nocbreak();
		i++;
	}
	return (fd);
}
