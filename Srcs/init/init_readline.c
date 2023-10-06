#include "../../Includes/defuser_wizard.h"

static int readline_getc(FILE *dummy)
{
	(void)dummy;
	g_data_s.rlncurses_s.input_avail = false;
	return (g_data_s.rlncurses_s.input);
}

// Not bothering with 'input_avail' and just returning 0 here seems to do the
// right thing too, but this might be safer across readline versions
static int readline_input_avail(void)
{
	return (g_data_s.rlncurses_s.input_avail);
}

static void readline_redisplay(void)
{
	print_cmd_win(g_data_s.dispopts_s.cmd_win, false);
}

static void line_handler(char *line)
{
	char	*temp;

	if (!line)
		exit_helper(g_data_s);
	temp = ft_strtrim(line, "\t\n\v\f\r ");
	free(line);
	if (!temp)
	{
		save_error(g_data_s.dispopts_s.bomb_output, BOMBOUT_BUFFER, BIN_NAME,
			__func__);
		return ;
	}
	if (!*temp)
		return (free(temp));
	line = temp;
	add_history(line);
	g_data_s.dispopts_s.cmd = line;
	if (*line != '@')
		exec_command(&g_data_s);
	free(g_data_s.dispopts_s.cmd);
	g_data_s.dispopts_s.cmd = NULL;
	main_layout(&g_data_s);
}

static int	interpret_keyseq(int a, int b)
{
	if (a != 1)
		return (0);
//	if (b == 65)
	fprintf(stderr, "%d %d yoyo\n", a, b);
	return (0);
}

void init_readline(void)
{
	// Disable completion. TODO: Is there a more robust way to do this?
	if (rl_bind_key('\t', rl_insert))
		fail_exit("rl_bind_key() invalid key '\\t'");
	
	// Bind the 'shift-up' and 'shift-down' sequences to the bomb output history
	// navigation
	if (rl_bind_keyseq(KEYSEQ_SHIFT_UP, interpret_keyseq))
		fail_exit("rl_bind_keyseq() invalid keyseq '" KEYSEQ_SHIFT_UP "'");
	if (rl_bind_keyseq(KEYSEQ_SHIFT_DOWN, interpret_keyseq))
		fail_exit("rl_bind_keyseq() invalid keyseq '" KEYSEQ_SHIFT_DOWN "'");

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
