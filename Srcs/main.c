#include "../Includes/defuser_wizard.h"

t_data	g_data_s;


int	main(void)
{
	// set locale attributes (including encoding) from the environment
	if (!setlocale(LC_ALL, ""))
		fail_exit("failed to set locale attributes from environment");

	init_portopts(&g_data_s.portopts_s);
	init_display(&g_data_s.rlncurses_s, &g_data_s.dispopts_s);
	g_data_s.history_lvl = 0;
	g_data_s.bomb_out_history = NULL;
	init_readline();

	attron(COLOR_PAIR(1));
	if (play_startup())
		exit_helper(g_data_s);
	attroff(COLOR_PAIR(1));

	g_data_s.goto_functions[0] = goto_layout_help;
	g_data_s.goto_functions[1] = goto_layout_1;
	g_data_s.goto_functions[2] = 0;

	g_data_s.goto_functions[1](&g_data_s);
//	goto_layout_1(&g_data_s);
//	open_usb_port(&conn_options);
//	main_layout(&g_data_s);
//	goto_layout_labyrinth(&g_data_s);
//	goto_layout_3(&g_data_s);
	while (1)
	{
		update_command(&g_data_s);
	}
	exit_helper(g_data_s);
	return (0);
}
