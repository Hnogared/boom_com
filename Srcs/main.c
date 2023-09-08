#include "../Includes/defuser_wizard.h"

int	init_disp_opts(dispopts **disp_options)
{
	WINDOW		*win;

    // Initialize ncurses
	win = initscr();
	cbreak();
	noecho();

	*disp_options = (dispopts *) calloc(1, sizeof(dispopts));
	if (!*disp_options)
		return(1);
	(*disp_options)->view = 1;
	(*disp_options)->prompt_char = '$';
	(*disp_options)->cmd[0] = 0;
	(*disp_options)->cmd_output[0] = 0;
	(*disp_options)->bomb_output[0] = 0;
	(*disp_options)->win = win;
	return (0);
}

void	init_colors(void)
{
	int	bg;

	start_color();
	bg = COLOR_BLACK;

    /* param 1     : color name
     * param 2, 3, 4 : rgb content min = 0, max = 1000 */
	if (can_change_color())
    {
		init_color(COLOR_BG, 75, 75, 150);
		init_color(COLOR_RED, 700, 0, 0);
		init_color(COLOR_BLACK, 0, 0, 0);
		init_color(COLOR_GREEN, 0, 1000, 200);
		bg = COLOR_BG;
	}

	init_pair(1, COLOR_RED, bg);
	init_pair(2, bg, COLOR_WHITE);
	init_pair(3, COLOR_RED, COLOR_WHITE);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);

	if (!can_change_color())
		return ;
}

int	init_termios(portopts **conn_options)
{
	(*conn_options)->fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
	memmove((*conn_options)->port, "/dev/ttyUSB0", 13);
	(*conn_options)->baudrate = 115200;
	set_termios_opt((*conn_options)->fd, 115200);
	return (0);
}

int	main(void)
{
    portopts	*conn_options;
	dispopts	*disp_options;

	conn_options = (portopts *) calloc(1, sizeof(portopts));
	if (!conn_options)
		exit_helper(NULL, NULL);
	conn_options->fd = -1;
	conn_options->port[0] = 0;
	
	if (init_disp_opts(&disp_options))
		exit_helper(conn_options, NULL);

	init_colors();

	attron(COLOR_PAIR(1));
	if (play_startup())
		exit_helper(conn_options, disp_options);
	attroff(COLOR_PAIR(1));

	init_termios(&conn_options);
	strncpy(disp_options->cmd_output, "Welcome to the bomb defuser UI.\n"
		"Please type one of the options below :\n\n"
		" # [1] Decrypt and access the bomb's system.\n"
		" # [2] Help (or type 'help')\n"
		" # [3] Exit (or type 'exit')\n", BIG_BUFFER - 2);
	disp_options->cmd_output[BIG_BUFFER - 1] = 0;
	menu_defusing(&conn_options, &disp_options);

	exit_helper(conn_options, disp_options);
	return (0);
}
