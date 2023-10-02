#include "../Includes/defuser_wizard.h"

t_data	g_data_s;

void	myrl_redisplay_function(void)
{
	mvprintw(LINES - 1, 0, "%s%s", rl_display_prompt, rl_line_buffer);
}

void	line_handler(char *line)
{
	if (!line)
	{
		fprintf(stderr, "yoyoyo\n");
		exit_helper(NULL, NULL);
	}
	if (*line)
		add_history(line);
//	myrl_redisplay_function();
}


int	main(void)
{
    portopts	*conn_options;
	dispopts	*disp_options;
	
	if (init_connopts(&g_data_s.conn_options)
		|| init_display(&g_data_s.disp_options))
		exit_helper(conn_options, NULL);
	init_readline();

	attron(COLOR_PAIR(1));
	if (play_startup())
		exit_helper(conn_options, disp_options);
	attroff(COLOR_PAIR(1));

//	goto_layout_1(&conn_options, &disp_options);
//	open_usb_port(&conn_options);
//	goto_layout_labyrinth(&conn_options, &disp_options);
	while (1)
	{
		update_command(&conn_options, &disp_options);
		menu_defusing(&conn_options, &disp_options);
	}
	exit_helper(conn_options, disp_options);
	return (0);
}
