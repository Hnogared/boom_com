#include "../Includes/defuser_wizard.h"

t_data	g_data_s;

int	main(void)
{
	init_portopts(&g_data_s.portopts_s);
	init_display(&g_data_s.rlncurses_s, &g_data_s.dispopts_s);
	init_readline();

	attron(COLOR_PAIR(1));
	if (play_startup())
		exit_helper(g_data_s.portopts_s, g_data_s.dispopts_s);
	attroff(COLOR_PAIR(1));

//	goto_layout_1(&conn_options, &disp_options);
//	open_usb_port(&conn_options);
//	goto_layout_labyrinth(&conn_options, &disp_options);
	while (1)
	{
		update_command(&g_data_s.portopts_s, &g_data_s.dispopts_s);
		menu_defusing(&g_data_s.portopts_s, &g_data_s.dispopts_s);
	}
	exit_helper(g_data_s.portopts_s, g_data_s.dispopts_s);
	return (0);
}
