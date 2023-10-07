#include "../Includes/defuser_wizard.h"

void resize(t_data *data_p)
{
	if (LINES >= 2)
	{
		CHECK(wresize, data_p->dispopts_s.cmd_win, 1, COLS);
		CHECK(mvwin, data_p->dispopts_s.cmd_win, LINES - 1, 0);
	}

	// Batch refreshes and commit them with doupdate()
	main_layout(data_p);
	print_cmd_win(data_p->dispopts_s.cmd_win, true);
	CHECK(doupdate);
}

void print_cmd_win(WINDOW *cmd_win, bool for_resize)
{
	size_t prompt_width;
	size_t cursor_col;

	prompt_width = strwidth(rl_display_prompt, 0);
	cursor_col = prompt_width + strnwidth(rl_line_buffer, rl_point, prompt_width);
	CHECK(werase, cmd_win);
	// This might write a string wider than the terminal currently, so don't
	// check for errors
	mvwprintw(cmd_win, 0, 0, "%s%s", rl_display_prompt, rl_line_buffer);
	// Hide the cursor if it lies outside the window. Otherwise it'll
	// appear on the very right.
	if (cursor_col >= (size_t) COLS)
		curs_set(0);
	else
	{
		CHECK(wmove, cmd_win, 0, cursor_col);
		curs_set(2);
	}
	// We batch window updates when resizing
	if (for_resize)
		CHECK(wnoutrefresh, cmd_win);
	else
		CHECK(wrefresh, cmd_win);
}
