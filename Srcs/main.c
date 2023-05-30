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
	(*disp_options)->cmd_len = 0;
	(*disp_options)->prompt_char = '$';
	(*disp_options)->cmd = (char *) calloc(255, sizeof(char));
	(*disp_options)->win = win;
	return (0);
}

int	main(void)
{
    portopts	*conn_options;
	dispopts	*disp_options;

	conn_options = (portopts *) calloc(1, sizeof(portopts));
	if (!conn_options)
		exit_helper(-1, NULL);
	conn_options->fd = -1;
	
	if (init_disp_opts(&disp_options))
		exit_helper(-1, NULL);

	if (play_startup())
		exit_helper(conn_options->fd, NULL);

	menu_defusing(&conn_options, &disp_options);

	endwin();
	close(conn_options->fd);
	free(conn_options);
	free(disp_options);
	return 0;
}
