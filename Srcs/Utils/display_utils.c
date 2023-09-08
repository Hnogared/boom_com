#include "../Includes/defuser_wizard.h"

void	main_menu_loading(char *message)
{
	int		i;
	int		delay;

	curs_set(0);
	i = 0;
	delay = 30;
	while (i * 3 <= COLS)
	{
		put_loading(message, "\t(OK)", LINES - 7, i * 3, COLS);
		cbreak();
		timeout(delay);
		getch();
		nocbreak();
		i++;
	}
	curs_set(1);
}

void	put_centered(char *str, int line, int width)
{
	int	padding;

	if (line > LINES)
		line = LINES;
	if (line >= 0)
		move(line, 0);
	padding = width / 2 - strlen(str) / 2;
	if (padding < 0)
		padding = 0;
	printw("%*s%s%*s", padding, "", str, padding, "");
}

void	put_separation(int line, int width)
{
	int		sep_len;
	char	*sep;

	sep = "__________________________________________________";
	sep_len = 50;
	if (line > LINES)
		line = LINES;
	if (line >= 0)
		move(line, 0);
	while (width > 0)
	{
		printw("%.*s", width, sep);
		width -= sep_len;
	}
}

void	put_loading(char *name, char *confirm, int line_pos, int progress, int length)
{
	int		progress_disp;
	char	*loading_bar;

	if (length < 1)
		return ;
	if (line_pos < 0)
		line_pos = 0;
	if (line_pos > LINES)
		line_pos = LINES - 2;
	loading_bar = "############################################################"
		"############################################################";
	move(line_pos, 0);
	if (name)
		printw("%s", name);
	if (progress >= length && confirm)
		printw("%s", confirm);
	progress_disp = progress * 100 / length;
	if (progress_disp > 100)
		progress_disp = 100;
	printw("\n[%03d%%][%.*s]", progress_disp, progress, loading_bar);
}
