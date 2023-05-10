#include "../Includes/defuser_wizard.h"

char	*g_port = NULL;

int	main(void)
{
    int				fd;
    struct termios	toptions;

    // Initialize ncurses
    initscr();
    cbreak();
    noecho();

    fd = -1;
    if (play_startup())
		exit_helper(fd, NULL);

    menu_defusing(&fd, &toptions);

    endwin();
    close(fd);

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
