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
	
	put_separation(15, COLS);

	mvprintw(18, 0, "CONFIGURING PORTS...");
	i = 1;
	while (i <= 4)
	{
		printw("\n [0%d] Ready", i++);
		getch();
	}
	
	put_separation(LINES - 4, COLS);
	attron(COLOR_PAIR(4));
	printw("%-*s", COLS, "System operational.");
	printw("%-*s", COLS, "");
	printw("%-*s", COLS, "I read and agree to the terms of service (press space to confirm) ");
	printw("%-*s", COLS, "");
	if (get_keypress() != ' ')
	{
		curs_set(1);
		mvprintw(LINES - 1, 0, "%-*s", COLS, "");
		printw("%-*s", COLS, "");
		mvprintw(LINES - 1, 0, "Disagree to the terms of service and exit ? (y/N) ");
		ch = get_keypress();
		attroff(COLOR_PAIR(4));
		if (ch == 'y' || ch == 'Y')
			return (1);
	}
	return (0);
}

int	open_port(portopts **conn_options, dispopts **disp_options)
{
	char	*error_type;

	error_type = "!> CONNECTION ERROR >> ";
	(*conn_options)->fd = open((*conn_options)->port, O_RDWR | O_NOCTTY);
	if ((*conn_options)->fd < 0)
	{
		curs_set(1);
		strncpy((*disp_options)->cmd_output, error_type, BIG_BUFFER);
		strncpy((*disp_options)->cmd_output + strlen(error_type), strerror(errno), BIG_BUFFER);
		(*disp_options)->cmd_output[BIG_BUFFER - 1] = 0;
		(*conn_options)->port[0] = 0;
	}
	else
		*(*conn_options)->toptions = set_termios_opt((*conn_options)->fd, cfgetispeed((*conn_options)->toptions));
	return ((*conn_options)->fd);
}

void	play_connect(portopts **conn_options, dispopts **disp_options)
{
	int		i;
	int		delay;

	clear();
	curs_set(0);
	attron(A_BOLD);
	attron(COLOR_PAIR(2));
	mvprintw(0, 0, "%-*s", COLS, "[BOMB DEFUSER] Establishing connection...");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(2));
	mvprintw(2, 0, "Please DO NOT at any time shut down the defuser during this process.\n");
	put_separation(-1, COLS);
	i = 0;
	while (i <= 60)
	{
		put_loading("OPENING PORT...", "\t(OK)", 5, i * 3, 60);
		if (i > 20 && i <= 40)
			put_loading("DECODING ENCRYPTION...", "\t(OK)", LINES / 2, (i - 20) * 3, 60);
		if (i > 40 && i <= 60)
			put_loading("ANALYSING FIRMWARE...", "\t(OK)", LINES - 7, (i - 40) * 3, 60);
		put_separation(LINES - 4, COLS);
		put_loading("PROGRESS", NULL, LINES - 2, i, 60);
		if (i == 55 && open_port(conn_options, disp_options) < 0)
			return ;
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
}
