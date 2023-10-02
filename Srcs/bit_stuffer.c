#include "../Includes/defuser_wizard.h"

static void	display_instructions(void)
{
	mvprintw(2, 0, " Programme pour corrompre les donnees "
		"d'une zone de memoire.\n");
	printw(" Utilisez [Z | Q | S | D] pour vous deplacer "
		"dans la memoire et la detruire.\n");
	printw(" Appuyez sur [ESC] pour quitter le programme.\n");
	attron(COLOR_PAIR(1));
	printw(" Restez sur le bon segment a l'aide des "
		"potentiometres de memoire de la bombe.\n");
	printw(" (un potentiometre est une molette que l'on peut tourner).\n");
	attroff(COLOR_PAIR(1));
	attron(A_BOLD);
}

static void	display_ui(void)
{
	int		i;

	curs_set(0);
	attron(A_BOLD);
	attron(COLOR_PAIR(2));
	mvprintw(0, 0, "%-*s", COLS, "bit_stuffer v. 1.2");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(2));
	display_instructions();
	attron(COLOR_PAIR(2));
	mvprintw(8, 0, "%-*s", COLS, "Derniers inputs");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(4));
	i = 6;
	while (++i < LINES - 5)
		printw("%-*s", COLS, "");
	put_separation(i, COLS);
	move(9, 0);
	curs_set(1);
}

static int	check_answer(t_portopts *portopts_p, t_dispopts *dispopts_p)
{
	int		size;
	char	temp[BIG_BUFFER];

	size = read(portopts_p->fd, temp, BIG_BUFFER - 1);
	if (size < -1)
		return (-1);
	if (strstr(temp, "end_lab"))
	{
		strncpy(dispopts_p->bomb_output, "�#erbonjou��memerror detected",
			BIG_BUFFER - 1);
		dispopts_p->bomb_output[BIG_BUFFER - 2] = 0;
		return (1);
	}
	fprintf(stderr, "%s\n", temp);
	return (0);
}

void	bit_stuffer(t_portopts *portopts_p, t_dispopts *dispopts_p)
{
	int		state;
	char	c;

	if (write(portopts_p->fd, "start_lab", 9) == -1)
	{
		strncpy(dispopts_p->bomb_output, "!> WRITING ERROR >> ",
			BIG_BUFFER - 2);
		strncpy(dispopts_p->bomb_output + 20, strerror(errno),
			BIG_BUFFER - 2);
		dispopts_p->bomb_output[BIG_BUFFER - 1] = 0;
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
		state = -1 * ((c == 'z' && write(portopts_p->fd, "moveZ", 5) == -1)
				|| (c == 'q' && write(portopts_p->fd, "moveQ", 5) == -1)
				|| (c == 's' && write(portopts_p->fd, "moveS", 5) == -1)
				|| (c == 'd' && write(portopts_p->fd, "moveD", 5) == -1));
		printw("%c", c * (c == 'z' || c == 'q' || c == 's' || c == 'd'));
	}
	if (state == 0)
		state = check_answer(portopts_p, dispopts_p);
	if (state < 0)
	{
		strncpy(dispopts_p->bomb_output, "!> WRITING ERROR >> ",
			BIG_BUFFER - 2);
		strncpy(dispopts_p->bomb_output + 20, strerror(errno),
			BIG_BUFFER - 2);
		dispopts_p->bomb_output[BIG_BUFFER - 1] = 0;
		goto_layout_labyrinth(g_data_s.dispopts_s.msg_win, portopts_p, dispopts_p);
		return ;
	}
	if (state)
		goto_layout_bytes(g_data_s.dispopts_s.msg_win, portopts_p, dispopts_p);
	else
		goto_layout_labyrinth(g_data_s.dispopts_s.msg_win, portopts_p, dispopts_p);
}
