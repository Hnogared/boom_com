#include "../Includes/defuser_wizard.h"

char     prompt = '$';
char    *g_port = NULL;

struct termios set_termios_opt(int fd, int baudrate)
{
    speed_t         to_set;
    struct termios	toptions;
 
    if (baudrate == 1 || baudrate == 0)
        to_set = B9600;
    else if (baudrate == 2)
        to_set = B38400;
    else
        to_set = B115200;
        
    tcgetattr(fd, &toptions);
    cfsetispeed(&toptions, to_set);
    cfsetospeed(&toptions, to_set);
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    toptions.c_cflag &= ~CRTSCTS;
    toptions.c_cflag |= CREAD | CLOCAL;
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY);
    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    toptions.c_oflag &= ~OPOST;
    toptions.c_cc[VMIN] = 0;
    toptions.c_cc[VTIME] = 0;

    tcsetattr(fd, TCSANOW, &toptions);
    usleep(1000*1000);
    tcflush(fd, TCIFLUSH);    

    return (toptions);
}

int get_baudrate(struct termios *toptions)
{
    if (cfgetispeed(toptions) == B115200)
        return (115200);
    if (cfgetispeed(toptions) == B38400)
        return (42000);
    if (cfgetispeed(toptions) == B9600)
        return (9600);
    return (0);
}

int    get_keypress()
{
    int ch;
    
    halfdelay(1);
    while (1)
    {
        ch = getch();
        if (ch != ERR)
        {
            nocbreak();
            return (ch);
        }
    }
    nocbreak();
    return (0);
}

char    **get_files_tab(char *directory)
{
    int             i;
    char            **files_tab;
    DIR             *d;
    struct dirent   *dir;
    
    d = opendir(directory);
    if (d) {
        i = 0;
        files_tab = (char **) calloc(10, sizeof(char *));
        while ((dir = readdir(d)) != NULL) {
            if (!left_strcmp(dir->d_name, "tty")
                && !(dir->d_name[3] >= '0' && dir->d_name[3] <= '9')
                && dir->d_name[3])
            {
                files_tab[i] = dir->d_name;
                i++;
            }   
        }
        closedir(d);
    }
    return (files_tab);
}

int exec_command(int *fd, char *command, int *view, struct termios *toptions)
{
    if (!left_strcmp("exit\n", command))
    {
        clear();
        close(*fd);
        free(command);
        mvprintw(0, 0, "Exiting bomb defusing assistant...");
        halfdelay(10);
        getch();
        nocbreak();
        clear();
        endwin();
        exit(0);
    }
    if (!left_strcmp("man\n", command))
    {
        system("less defuser_man.txt");
        return (1);
    }
    if (check_help_cmds(command, view))
        return (0);
    if (check_view_cmds(command, view) || check_conn_cmds(fd, command, view, toptions))
        return (1);
    if (*view)
    {
        printw("ERROR >> Unknown command : %s\n", command);
        printw("Type 'help' for a list of all defusing assistant commands or get the manual (cmd 'man').");
    }
    return (0);
}

int main(void)
{
    int				fd;
    struct termios	toptions;

    // Initialize ncurses
    initscr();
    cbreak();
    noecho();

    fd = 0;
    if (play_startup())
    {
        clear();
        mvprintw(0, 0, "Exiting bomb defusing helper...");
        halfdelay(5);
        getch();
        nocbreak();
        endwin();
        exit(1);
    }

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
