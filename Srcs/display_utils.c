#include "../Includes/defuser_wizard.h"

void	put_separation(int line)
{
	char	*sep;

	sep = "_________________________________________________________________________________________________________________________";
	if (line < 0)
		line = 0;
	if (line > LINES)
		line = LINES;
	mvprintw(line, 0, "%.*s", COLS, sep);
}
