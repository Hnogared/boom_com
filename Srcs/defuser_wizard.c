#include "../Includes/defuser_wizard.h"

char	*g_port = NULL;

int	main(void)
{
    portopts	*conn_options;

    // Initialize ncurses
    initscr();
    cbreak();
    noecho();

	conn_options = (portopts *) calloc(1, sizeof(portopts));
    conn_options->fd = -1;
    conn_options->toptions = NULL;
	if (play_startup())
		exit_helper(conn_options->fd, NULL);

    menu_defusing(conn_options);

    endwin();
    close(conn_options->fd);

    return 0;
}

/*
int add_to_buf(char *buf)
{
    int i = 0;
    int j = 0;

    while (g_buf[i])
        i++;
    while (buf[j]){
        g_buf[i + j] = buf[j];
        j++;
    }
    return (0);
}
*/
