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

int check_help_cmds(char *command, int *view)
{
    if (!left_strcmp("help\n", command))
    {
        if (*view)
        {
            printw("List of the available important commands :\n\n");
            printw("/!\\ WARNING /!\\\n");
            printw("Don't forget that these commands are to be used only for the defuser, the ones for the bombwon't be the same.\n");
            printw("To send a command to the bomb, it must be preceded by a '@' character.\n");
            printw("For more information, type 'help_cmd' or refer to the manual (cmd 'man').\n\n");
            printw("# help\t\tShow this menu.\n");
            printw("# man\t\tOpen the manual.\n");
            printw("# exit\t\tClose *CONFEDERATION BOMB DEFUSER v4.6*.\n");
            printw("# help cmd\tQuick guide on how to send commands through the defuser.\n");
            printw("# help nav\tDisplay the commands for navigating through the defuser.\n");
            printw("# help connect\tQuick guide on how to connect to a bomb.\n");
            printw("# help hacks\tQuick guide on hacking attacks on electronics/informatics.\n");
        }
        return (1);
    }
    else if (!left_strcmp("help cmd\n", command))
    {
        if (*view)
        {
            printw("The *CONFEDERATION BOMB DEFUSER v4.6* is an interface for using disarming tools on\nterrorist devices.\n\n");
            printw("There are two types of commands that can be sent through the program :\n");
            printw(" * Direct commands that will be interpreted by the defuser itself.\n");
            printw(" * Commands starting with '@' that will be sent to the bomb to interact with it.\n\n");
            printw("Please be aware that the Confederation cannot be taken as responsible for the outcome of\nany command sent to a bomb.\n\n");
            printw("You can type 'help' or read the manual (cmd 'man') for more information.\n");
        }
        return (1);
    }
    else if (!left_strcmp("help nav\n", command))
    {
        if (*view)
        {
            printw("It is possible to navigate and rearrange the workspace as needed.\n");
            printw("This can be achieved by using the following commands.\n\n");
            printw("NOTE : You can quickly cycle between workspace arrangements by pressing 'TAB'.\n\n");
            printw("List of commands to navigate through the GUI of the defuser :\n\n");
            printw("# exit\t\t\tClose *CONFEDERATION BOMB DEFUSER v4.6*.\n");
            printw("# set-view bomb\t\tSwitch to bomb view only.\n");
            printw("# set-view difuser\tSwitch to defuser view only.\n");
            printw("# set-view split\tSplit the screen horizontally between the bomb and defuser views.\n");
        }
        return (1);
    }
    else if (!left_strcmp("help connect\n", command))
    {
        if (*view)
        {
            printw("Quick guide on how to setup a connection between your defuser and a bomb.\n\n");
            printw("1. Connect one of the defuser's USB ports to the device to disarm.\n");
            printw("2. Run 'set_port' and select an output USB port.\n");
            printw("3. Following the port, use 'set_baudrate' to select a baud rate.\n");
            printw("4. You should then get a connection confirmation in the 'BOMB INTERPRETOR' tab.\n");
            printw("Depending on the bomb type, you might get an audio/visual confirmation from it as well.\n\n");
            printw("ISSUES:\n");
            printw("* If connecting doesn't seem to have succeeded, check your port and/or baud rate settings.\n");
            printw("* A connection through the correct USB port will link with the bomb.\n");
            printw("However, an invalid baud rate will lead to communication issues with the device.\n");
            printw("If this seems to be the case for you, try out different rates through the same port.\n\n");
            printw("More information on connecting to a bomb in the manual (cmd 'man').\n");
            printw("# set-port\tChange the defuser output USB port.\n");
            printw("# set-baudrate\tChange the port data transfer rate.\n");
        }
        return (1);
    }
    else if (!left_strcmp("help hacks\n", command))
    {
        if (*view)
        {
            printw("Quick guide on hacking attacks on electronics/informatics usign the defuser.\n\n");
            printw("1. Look for vulnerabilities : outdated software, open ports.\n");
            printw("2. Select one of the tools we provide to exploit a potential vulnerability.\n\n");
            printw("Available hacking programs:\n");
            printw("# firewall-ext [ID]\tProgram to overload a component's firewall.\n");
            printw("# bytestuffer [ID]\tMake a given component's memory overflow.\n");
            printw("More information on hacking into electronic/informatic systems in the manual (cmd 'man').\n");
            
        }
        return (1);
    }
    return (0);
}

int check_view_cmds(char *command, int *view)
{
    if (!left_strcmp("set-view bomb\n", command))
    {
        *view = 0;
        return (1);
    }
    else if (!left_strcmp("set-view defuser\n", command))
    {
        *view = 1;
        return (1);
    }
    else if (!left_strcmp("set-view split\n", command))
    {
        *view = 2;
        return (1);
    }
    return (0);
}

int check_conn_cmds(int *fd, char *command, int *view, struct termios *toptions)
{
    int     baudrate;
    char    *temp;

    if (!left_strcmp("set-port\n", command))
    {
        temp = menu_port_select();
        if (temp)
            g_port = ft_strjoin("/dev/", temp);
        if (g_port && *fd && cfgetispeed(toptions))
            *fd = play_connect(g_port, cfgetispeed(toptions), toptions);
        return (1);
    }
    else if (!left_strcmp("set-baudrate\n", command))
    {
        if (!g_port && *view)
        {
            printw("ERROR >> Please select a port before setting up the baud rate.");
            return (0);
        }
        baudrate = menu_baudrate_select();
        if (baudrate)
        {
            if (!*fd)
                *fd = play_connect(g_port, cfgetispeed(toptions), toptions);
            *toptions = set_termios_opt(*fd, baudrate);
        }
        return (1);
    }
    return (0);
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
