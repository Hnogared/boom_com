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

	if (play_startup())
		exit_helper(conn_options, disp_options);

	menu_defusing(&conn_options, &disp_options);

	exit_helper(conn_options, disp_options);
	return 0;
}
