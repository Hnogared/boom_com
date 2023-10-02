#include "../Includes/defuser_wizard.h"

static void	init_colors(dispopts_t *disp_options)
{
	int	bg;

	bg = COLOR_BLACK;

    /* param 1     : color name
     * param 2, 3, 4 : rgb content min = 0, max = 1000 */
	if (can_change_color())
    {
		init_color(COLOR_BG, 75, 75, 150);
		init_color(COLOR_RED, 700, 0, 0);
		init_color(COLOR_BLACK, 0, 0, 0);
		init_color(COLOR_GREEN, 0, 1000, 200);
		bg = COLOR_BG;
	}
	if (has_colors)
	{
		start_color();
		init_pair(1, COLOR_RED, bg);
		init_pair(2, bg, COLOR_WHITE);
		init_pair(3, COLOR_RED, COLOR_WHITE);
		init_pair(4, COLOR_GREEN, COLOR_BLACK);
        CHECK(wbkgd, disp_options->cmd_win, COLOR_PAIR(4));
	}
    else
        CHECK(wbkgd, disp_options->cmd_win, A_STANDOUT);
    CHECK(wrefresh, disp_options->cmd_win);
}

static void init_ncurses(dispopts_t *disp_options)
{
	WINDOW	*win;

    disp_options->win = initscr();
	if (!disp_options->win)
        fail_exit("Failed to initialize ncurses");
    visual_mode = true;

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

    if (LINES >= 2) {
        disp_options->msg_win = newwin(LINES - 1, COLS, 0, 0);
        disp_options->cmd_win = newwin(1, COLS, LINES - 1, 0);
    }
    else {
        // Degenerate case. Give the windows the minimum workable size to
        // prevent errors from e.g. wmove().
        disp_options->msg_win = newwin(1, COLS, 0, 0);
        disp_options->cmd_win = newwin(1, COLS, 0, 0);
    }
    if (!msg_win || !cmd_win)
        fail_exit("Failed to allocate windows");

    // Allow strings longer than the message window and show only the last part
    // if the string doesn't fit
    CHECK(scrollok, msg_win, TRUE);
}

void	init_display(t_dispopts *disp_options)
{
	disp_options->view = 1;
	disp_options->prompt_char = '$';
	disp_options->cmd[0] = 0;
	disp_options->cmd_output[0] = 0;
	disp_options->bomb_output[0] = 0;
	disp_options->win = win;
	init_colors(disp_options);
	init_ncurses(disp_options);
	return (0);
}

void	init_portopts(t_portopts *port_options)
{
	port_options->fd = -1;
	port_options->port[0] = 0;
}

void init_readline(void)
{
    // Disable completion. TODO: Is there a more robust way to do this?
    if (rl_bind_key('\t', rl_insert))
        fail_exit("Invalid key passed to rl_bind_key()");

    // Let ncurses do all terminal and signal handling
    rl_catch_signals = 0;
    rl_catch_sigwinch = 0;
    rl_deprep_term_function = NULL;
    rl_prep_term_function = NULL;

    // Prevent readline from setting the LINES and COLUMNS environment
    // variables, which override dynamic size adjustments in ncurses. When
    // using the alternate readline interface (as we do here), LINES and
    // COLUMNS are not updated if the terminal is resized between two calls to
    // rl_callback_read_char() (which is almost always the case).
    rl_change_environment = 0;

    // Handle input by manually feeding characters to readline
    rl_getc_function = readline_getc;
    rl_input_available_hook = readline_input_avail;
    rl_redisplay_function = readline_redisplay;

    rl_callback_handler_install("> ", line_handler);
}
