#include "../Includes/defuser_wizard.h"

int	play_startup(void)
{
	int		i;
	int		ch;
	char	*padding;

	padding = "----------------------";
	
	clear();
	curs_set(0);
	mvprintw(0, 0, "Booting system");
	timeout(500);
	getch();
	clear();
	
	cbreak();
	timeout(30);
		
	i = 0;
	while(i < 20)
	{
		mvprintw(0, 0, "%s ======[%.*s", padding, i * 3, "CONFEDERATION BOMB DEFUSER v4.6");
		printw("]====== %s", padding);
		getch();
		i++;
	}
	
	i = 0;
	while (i <= 120)
	{
		if (i <= 60)
			put_loading("LOADING SCRIPTS...", "\t(OK)", 3, i, 60);
		if (i > 60 && i <= 90)
			put_loading("CHECKING FOR UPDATES...", "\t(UP TO DATE)", 6, (i - 60) * 2, 60);
		if (i > 90)
		{
			put_loading("LOADING INTERFACE...", "\t(OK)", 9, (i - 90) * 2, 60);
			put_loading("LOADING MANUAL...", "\t(OK)", 12, (i - 90) * 3, 60);
		}
		getch();
		i++;
	}
	
	timeout(100);
	
	put_separation(15);

	mvprintw(18, 0, "CONFIGURING PORTS...");
	i = 1;
	while (i <= 4)
	{
		printw("\n [0%d] Ready", i++);
		getch();
	}
	
	put_separation(LINES - 6);
	
	mvprintw(LINES - 4, 0, "System operational.\n\n");
	curs_set(1);
	printw("I read and agree to the terms of service (press space to confirm) ");
	
	nocbreak();
	if (get_keypress() != ' ')
	{
		mvprintw(LINES - 2, 0, "Disagree to the terms of service and exit ? (y/N) ");
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
	
	i = 0;
	clear();
	curs_set(0);
	mvprintw(0, 0, "[BOMB DEFUSER] Establishing connection...\n\n");
	printw("Please DO NOT at any time shut down the defuser during this process.");
	put_separation(-1);
	while (i <= 60)
	{
		put_loading("OPENING PORT...", "\t(OK)", 5, i * 3, 60);
		if (i > 20 && i <= 40)
			put_loading("DECODING ENCRYPTION...", "\t(OK)", LINES / 2, (i - 20) * 3, 60);
		if (i > 40 && i <= 60)
			put_loading("ANALYSING FIRMWARE...", "\t(OK)", LINES - 7, (i - 40) * 3, 60);
		put_separation(LINES - 4);
		put_loading("PROGRESS", NULL, LINES - 2, i, 60);

		// Open serial port
		if (i == 55)
			fd = open(port_name, O_RDWR | O_NOCTTY);
		if (fd == -1 && i >= 55)
		{
			curs_set(1);
			endwin();
			perror("open");
			printf("%s\n", port_name);
			return (-1);
		}
		else if (i == 55)
			*toptions = set_termios_opt(fd, baudrate);
		
		delay = 100;
		if (i < 30 || i > 45)
			delay = 75;

		cbreak();
		timeout(delay);
		getch();
		nocbreak();
		i++;
	}
	curs_set(1);
	return (fd);
}
