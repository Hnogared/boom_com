#include "../Includes/defuser_wizard.h"

static int readline_getc(FILE *dummy)
{
	(void)dummy;
	g_data_s.rlncurses_s.input_avail = false;
	return g_data_s.rlncurses_s.input;
}

// Not bothering with 'input_avail' and just returning 0 here seems to do the
// right thing too, but this might be safer across readline versions
static int readline_input_avail(void)
{
	return g_data_s.rlncurses_s.input_avail;
}

static void readline_redisplay(void)
{
	cmd_win_redisplay(g_data_s.dispopts_s.cmd_win, false);
}

static int	is_whitespace_str(char *str)
{
	if (!str || !*str)
		return (1);
	while (*str && (*str == 32 || (*str >= 9 && *str <= 13) || *str == 127))
		str++;
	return (!*str);
}

static void line_handler(char *line)
{
	if (!line)
		exit_helper(g_data_s.portopts_s, g_data_s.dispopts_s);
	if (is_whitespace_str(line))
		return ;
	add_history(line);
	g_data_s.dispopts_s.cmd  = line;
	exec_command(&g_data_s.portopts_s, &g_data_s.dispopts_s);
	free(g_data_s.dispopts_s.cmd);
	g_data_s.dispopts_s.cmd = NULL;
	menu_defusing(g_data_s.dispopts_s.msg_win, &g_data_s.portopts_s, &g_data_s.dispopts_s);
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

	rl_callback_handler_install(PROMPT " ", line_handler);
}