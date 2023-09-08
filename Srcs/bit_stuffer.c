#include "../Includes/defuser_wizard.h"

void	bit_stuffer(portopts **conn_options, dispopts **disp_options)
{
	char	c;

	clear();
	curs_set(0);
	attron(A_BOLD);
	attron(COLOR_PAIR(2));
	mvprintw(0, 0, "%-*s", COLS, "bit_stuffer v. 1.2");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(2));
	mvprintw(2, 0, " Program to corrupt memory data.\n");
	printw(" Use your keys [Z | Q | S | D] to move arround the memory and destroy it\n");
	printw(" Press the [ESC] key once you are finished\n");
	put_separation(5, COLS);
	attron(A_BOLD);
	attron(COLOR_PAIR(2));
	printw(" Last inputs\n");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(2));
	while (true)
	{
		c = get_keypress();
		if (c == '\e')
			break ;
		if (c == 'z')
			write((*conn_options)->fd, "moveZ", 5);
		if (c == 'q')
			write((*conn_options)->fd, "moveQ", 5);
		if (c == 's')
			write((*conn_options)->fd, "moveS", 5);
		if (c == 'd')
			write((*conn_options)->fd, "moveD", 5);
		printw("%c", c);
	}
	put_separation(LINES - 2, COLS);
	curs_set(1);
	goto_layout_labyrinth(conn_options, disp_options);
}
