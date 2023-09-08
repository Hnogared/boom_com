#include "../Includes/defuser_wizard.h"

static void	display_ui(void)
{
	int		i;
	
	curs_set(0);
	attron(A_BOLD);
	attron(COLOR_PAIR(2));
	mvprintw(0, 0, "%-*s", COLS, "bit_stuffer v. 1.2");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(2));
	mvprintw(2, 0, " Program to corrupt memory data.\n");
	printw(" Use your keys [Z | Q | S | D] to move arround the memory and destroy it.\n");
	printw(" Press the [ESC] key once you are finished.\n");
	attron(A_BOLD);
	attron(COLOR_PAIR(2));
	mvprintw(6, 0, "%-*s", COLS, "Last inputs");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(4));
	i = 6;
	while (++i < LINES - 5)
		printw("%-*s", COLS, "");
	put_separation(i, COLS);
	move(7, 0);
	curs_set(1);
}

static int	check_answer(portopts **conn_options, dispopts **disp_options)
{
	int		size;
	char	temp[BIG_BUFFER];

	size = read((*conn_options)->fd, temp, BIG_BUFFER - 1);
	if (size < -1)
		return (-1);
	if (strstr(temp, "end_lab"))
	{
		strncpy((*disp_options)->bomb_output, "�#erbonjou��memerror detected",
			BIG_BUFFER - 1);
		(*disp_options)->bomb_output[BIG_BUFFER - 2] = 0;
		return (2);
	}
	return (0);
}

void	bit_stuffer(portopts **conn_options, dispopts **disp_options)
{
	int		state;
	char	c;

	if (write((*conn_options)->fd, "start_lab", 9) == -1)
	{
		strncpy((*disp_options)->bomb_output, "!> WRITING ERROR >> ", BIG_BUFFER);
		strncpy((*disp_options)->bomb_output + 20, strerror(errno), BIG_BUFFER);
		(*disp_options)->bomb_output[BIG_BUFFER - 1] = 0;
		return ;
	}
	clear();
	display_ui();
	state = 0;
	while (state == 0)
	{
		c = get_keypress();
		if (c == '\e')
			break ;
		state = -1 * ((c == 'z' && write((*conn_options)->fd, "moveZ", 5) == -1)
			|| (c == 'q' && write((*conn_options)->fd, "moveQ", 5) == -1)
			|| (c == 's' && write((*conn_options)->fd, "moveS", 5) == -1)
			|| (c == 'd' && write((*conn_options)->fd, "moveD", 5) == -1));
		printw("%c", c * (c == 'z' || c == 'q' || c == 's' || c == 'd'));
		state += check_answer(conn_options, disp_options);
	}
	if (state < 0)
	{
		strncpy((*disp_options)->bomb_output, "!> WRITING ERROR >> ", BIG_BUFFER);
		strncpy((*disp_options)->bomb_output + 20, strerror(errno), BIG_BUFFER);
		(*disp_options)->bomb_output[BIG_BUFFER - 1] = 0;
		goto_layout_labyrinth(conn_options, disp_options);
		return ;
	}
	if (state)
		goto_layout_bytes(conn_options, disp_options);
	else
		goto_layout_labyrinth(conn_options, disp_options);
}
