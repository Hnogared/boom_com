#include "../../Includes/defuser_wizard.h"

static void	display_instructions(void)
{
	mvprintw(2, 0, " Programme pour corrompre les donnees "
		"d'une zone de memoire.\n"
		" Utilisez [Z | Q | S | D] pour vous deplacer "
		"dans la memoire et la detruire.\n"
		" Appuyez sur [ESC] pour quitter le programme.\n");
	attron(COLOR_PAIR(1));
	printw(" Restez sur le bon segment a l'aide des "
		"potentiometres de memoire de la bombe.\n"
		" (un potentiometre est une molette que l'on peut tourner).\n");
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

static void	check_bomb_out(t_data *data_p, char *func_name)
{
	int		size;
	char	temp[BOMBOUT_BUFFER];

	if (!data_p || data_p->portopts_s.fd < 0)
		return ;
	size = read(data_p->portopts_s.fd, temp, BOMBOUT_BUFFER - 1);
	if (size == -1)
	{
		save_error(data_p->dispopts_s.bomb_output, BOMBOUT_BUFFER,
			" ! " BIN_NAME, func_name);
		goto_layout_labyrinth(data_p);
		return ;
	}
	temp[size] = 0;
	if (!strstr(temp, "end_lab"))
	{
		goto_layout_labyrinth(data_p);
		return ;
	}
	safer_strncpy(data_p->dispopts_s.bomb_output, "�#erbonjou��memerror detected",
		BOMBOUT_BUFFER, 30);
	goto_layout_bytes(data_p);
}

void	bit_stuffer(t_data *data_p)
{
	int		state;
	char	c;

	if (!data_p)
		return ;
	if (write(data_p->portopts_s.fd, "start_lab", 9) == -1)
	{
		save_error(data_p->dispopts_s.bomb_output, BOMBOUT_BUFFER, " ! " BIN_NAME,
			__func__);
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
		state = -1 * ((c == 'z' && write(data_p->portopts_s.fd, "moveZ", 5) == -1)
				|| (c == 'q' && write(data_p->portopts_s.fd, "moveQ", 5) == -1)
				|| (c == 's' && write(data_p->portopts_s.fd, "moveS", 5) == -1)
				|| (c == 'd' && write(data_p->portopts_s.fd, "moveD", 5) == -1));
		if (state == -1)
		{
			save_error(data_p->dispopts_s.bomb_output, BOMBOUT_BUFFER,
				" ! " BIN_NAME, __func__);
			goto_layout_labyrinth(data_p);
			return ;
		}
		printw("%c", c * (c == 'z' || c == 'q' || c == 's' || c == 'd'));
	}
	check_bomb_out(data_p, __func__);
}
