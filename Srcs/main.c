#include "../Includes/defuser_wizard.h"

int	main(void)
{
    portopts	*conn_options;
	dispopts	*disp_options;

    // Initialize ncurses
	initscr();
	cbreak();
	noecho();

	conn_options = (portopts *) calloc(1, sizeof(portopts));
	if (!conn_options)
		exit_helper(-1, NULL);
	conn_options->fd = -1;
	
	disp_options = (dispopts *) calloc(1, sizeof(dispopts));
	if (!disp_options)
		exit_helper(-1, NULL); 
	disp_options->view = 1;

	if (play_startup())
		exit_helper(conn_options->fd, NULL);

	menu_defusing(&conn_options, &disp_options);

	endwin();
	close(conn_options->fd);
	free(conn_options);
	free(disp_options);
	return 0;
}
