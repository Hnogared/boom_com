#include "../../Includes/defuser_wizard.h"

static void	init_colors(t_dispopts *dispopts_p)
{
	int	bg;

	bg = COLOR_BLACK;

	/* param 1	 : color name
	 * param 2, 3, 4 : rgb content min = 0, max = 1000 */
	if (can_change_color())
	{
		init_color(COLOR_BG, 75, 75, 150);
		init_color(COLOR_RED, 700, 0, 0);
		init_color(COLOR_BLACK, 0, 0, 0);
		init_color(COLOR_GREEN, 0, 1000, 200);
		bg = COLOR_BG;
	}
	if (has_colors())
	{
		start_color();
		init_pair(1, COLOR_RED, bg);
		init_pair(2, bg, COLOR_WHITE);
		init_pair(3, COLOR_RED, COLOR_WHITE);
		init_pair(4, COLOR_GREEN, bg);
		CHECK(wbkgd, dispopts_p->cmd_win, COLOR_PAIR(4));
	}
	else
		CHECK(wbkgd, dispopts_p->cmd_win, A_STANDOUT);
	CHECK(wrefresh, dispopts_p->cmd_win);
}

static void init_ncurses(t_rlncurses *rlncurses_p, t_dispopts *dispopts_p)
{
	dispopts_p->win = initscr();
	if (!dispopts_p->win)
		fail_exit("Failed to initialize ncurses");
	rlncurses_p->visual_mode = true;

	CHECK(cbreak);
	CHECK(noecho);
	CHECK(nonl);
	CHECK(intrflush, NULL, FALSE);
	// Do not enable keypad() since we want to pass unadulterated input to
	// readline

	// Explicitly specify a "very visible" cursor to make sure it's at least
	// consistent when we turn the cursor on and off (maybe it would make sense
	// to query it and use the value we get back too). "normal" vs. "very
	// visible" makes no difference in gnome-terminal or xterm. Let this fail
	// for terminals that do not support cursor visibility adjustments.
	curs_set(2);

	if (LINES >= 2)
		dispopts_p->cmd_win = newwin(1, COLS, LINES - 1, 0);
	else
		// Degenerate case. Give the windows the minimum workable size to
		// prevent errors from e.g. wmove().
		dispopts_p->cmd_win = newwin(1, COLS, 0, 0);
	if (!dispopts_p->cmd_win)
		fail_exit("Failed to allocate windows");
}

void	init_display(t_rlncurses *rlncurses_p, t_dispopts *dispopts_p)
{
	dispopts_p->view = 1;
	dispopts_p->prompt_char = '$';
	dispopts_p->cmd = NULL;
	dispopts_p->cmd_output[0] = 0;
	dispopts_p->bomb_output[0] = 0;
	init_ncurses(rlncurses_p, dispopts_p);
	init_colors(dispopts_p);
}

void	init_portopts(t_portopts *port_options)
{
	port_options->fd = -1;
	port_options->port[0] = 0;
}
