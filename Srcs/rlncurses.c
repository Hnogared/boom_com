// copyright (c) 2015-2019, ulf magnusson
// spdx-license-identifier: isc

#include "../Includes/defuser_wizard.h"

// Calculates the cursor column for the readline window in a way that supports
// multibyte, multi-column and combining characters. readline itself calculates
// this as part of its default redisplay function and does not export the
// cursor column.
//
// Returns the total width (in columns) of the characters in the 'n'-byte
// prefix of the null-terminated multibyte string 's'. If 'n' is larger than
// 's', returns the total width of the string. Tries to emulate how readline
// prints some special characters.
//
// 'offset' is the current horizontal offset within the line. This is used to
// get tab stops right.
//
// Makes a guess for malformed strings.
size_t	strnwidth(const char *s, size_t n, size_t offset)
{
	mbstate_t shift_state;
	wchar_t wc;
	size_t wc_len;
	size_t width = 0;

	// Start in the initial shift state
	memset(&shift_state, '\0', sizeof shift_state);

	for (size_t i = 0; i < n; i += wc_len) {
		// Extract the next multibyte character
		wc_len = mbrtowc(&wc, s + i, MB_CUR_MAX, &shift_state);
		switch (wc_len) {
		case 0:
			// Reached the end of the string
			goto done;

		case (size_t)-1: case (size_t)-2:
			// Failed to extract character. Guess that each character is one
			// byte/column wide each starting from the invalid character to
			// keep things simple.
			width += strnlen(s + i, n - i);
			goto done;
		}

		if (wc == '\t')
			width = ((width + offset + 8) & ~7) - offset;
		else
			// TODO: readline also outputs ~<letter> and the like for some
			// non-printable characters
			width += iswcntrl(wc) ? 2 : max(0, wcwidth(wc));
	}

done:
	return width;
}

noreturn void	fail_exit(const char *msg)
{
	// Make sure endwin() is only called in visual mode. As a note, calling it
	// twice does not seem to be supported and messed with the cursor position.
	if (g_data_s.rlncurses_s.visual_mode)
		endwin();
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}

// Like strnwidth, but calculates the width of the entire string
size_t	strwidth(const char *s, size_t offset)
{
	return strnwidth(s, SIZE_MAX, offset);
}

void	forward_to_readline(char c, t_rlncurses *rlncurses_p)
{
	rlncurses_p->input = c;
	rlncurses_p->input_avail = true;
	rl_callback_read_char();
}

void deinit_ncurses(t_rlncurses *rlncurses_p, t_dispopts dispopts_s)
{
	CHECK(delwin, dispopts_s.msg_win);
	CHECK(delwin, dispopts_s.cmd_win);
	CHECK(endwin);
	rlncurses_p->visual_mode = false;
}

void deinit_readline(void)
{
	rl_callback_handler_remove();
}

void resize(t_portopts *portopts_p, t_dispopts *dispopts_p)
{
	if (LINES >= 2)
	{
		CHECK(wresize, dispopts_p->msg_win, LINES - 2, COLS);
		CHECK(wresize, dispopts_p->cmd_win, 1, COLS);
		CHECK(mvwin, dispopts_p->cmd_win, LINES - 1, 0);
	}

	// Batch refreshes and commit them with doupdate()
	menu_defusing(dispopts_p->msg_win, portopts_p, dispopts_p);
	cmd_win_redisplay(dispopts_p->cmd_win, true);
	CHECK(doupdate);
}
/*
int main(void)
{
	// Set locale attributes (including encoding) from the environment
	if (!setlocale(LC_ALL, ""))
		fail_exit("Failed to set locale attributes from environment");

	init_ncurses();
	init_readline();

	do {
		// Using getch() here instead would refresh stdscr, overwriting the
		// initial contents of the other windows on startup
		int c = wgetch(cmd_win);

		switch (c) {
		case KEY_RESIZE:
			resize(g_data_s.portopts_s, g_data_s.dispopts_s);
			break;

		// Ctrl-L -- redraw screen
		case '\f':
			// Makes the next refresh repaint the screen from scratch
			CHECK(clearok, curscr, TRUE);
			// Resize and reposition windows in case that got messed up somehow
			resize();
			break;

		default:
			forward_to_readline(c);
		}
	} while (!should_exit);

	deinit_ncurses();
	deinit_readline();

	puts("Shut down cleanly");
}*/
