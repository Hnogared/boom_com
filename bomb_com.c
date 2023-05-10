#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <dirent.h>

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

int chrstr(char c, char *str)
{
    int i;

    i = -1;
    while (str[++i]){
        if (str[i] == c)
            return (1);
    }
    return (0);
}

int left_strcmp(char *str1, char *str2)
{
    while (str1 && str2 && *str1 == *str2)
    {
        str1++;
        str2++;
    }
    if (str2 && !*str2)
        return (0);
    return (1);
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

char    *crop(char *str)
{
    char    *start;
    
    start = str;
    while (str && *str && *str != '$')
        str++;
    if (str && *str == '$' && *(str + 1))
        *str++ = 0;
    while (str && *str)
        *str++ = 0;
    return (start);
}

char    *ft_strjoin(char *dest, char *src)
{
    int     i;
    int     j;
    char    *res;
    
    i = 0;
    j = 0;
    while (dest && dest[i])
        i++;
    while (src && src[j])
        j++;
    res = (char *) malloc((i + j + 1) * sizeof(char));
    if (!res)
        return (NULL);
    i = 0;
    while (dest && *dest)
        res[i++] = *dest++;
    while (src && *src)
        res[i++] = *src++;
    res[i] = 0;
    return (res);
}

void    free_str_tab(char **str_tab)
{
    int i;
    
    i = 0;
    while (str_tab[i])
    {
        if (str_tab[i])
            free(str_tab[i]);
        i++;
    }
    free(str_tab);
}

int    play_startup()
{
    int     i;
    int     ch;
    char    *padding;
    char    *loading_bar;
    
    padding = "----------------------";
    loading_bar = "############################################################";
    
    clear();
    
    mvprintw(0, 0, "Booting system");
    halfdelay(5);
    getch();
    nocbreak();
    
    clear();
    
    cbreak();
    timeout(100);
        
    i = 0;
    while(i < 20)
    {
        mvprintw(0, 0, "%s ======[%.*s", padding, i * 3, "CONFEDERATION BOMB DEFUSER v4.6");
        printw("]====== %s", padding);
        getch();
        i++;
    }
    
    timeout(50);
    
    i = 0;
    while (i <= 60)
    {
        mvprintw(3, 0, "LOADING SCRIPTS...");
        if (i == 60)
            printw(" (OK)");
        printw("\n[%d%%][%.*s]", i * 100 / 60, i, loading_bar);
        getch();
        i++;
    }
    
    i = 0;
    while (i <= 30)
    {
        mvprintw(6, 0, "CHECKING FOR UPDATES...");
        if (i == 30)
            printw(" (Up to date)");
        printw("\n[%d%%][%.*s]", i * 100 / 30, i * 2, loading_bar);
        getch();
        i++;
    }
    
    i = 0;
    while (i <= 30)
    {
        mvprintw(9, 0, "LOADING INTERFACE...");
        if (i >= 20)
            printw(" (OK)");
        printw("\n[%d%%][%.*s]", i * 3 * 100 / 30 > 100 ? 100 : i * 3 * 100 / 30, i * 3, loading_bar);
        mvprintw(12, 0, "LOADING MANUAL...");
        if (i == 30)
            printw(" (OK)");
        printw("\n[%d%%][%.*s]", i * 100 / 30, i * 2, loading_bar);
        getch();
        i++;
    }
    
    timeout(100);
    
    i = 0;
    while (i <= 4)
    {
        mvprintw(15, 0, "CONFIGURING PORTS...");
        if (i == 4)
            printw(" (OK)");
        if (i >= 1)
            printw("\n [01] Open");
        if (i >= 2)
            printw("\n [02] Open");
        if (i >= 3)
            printw("\n [03] Open");
        if (i == 4)
            printw("\n [04] Open");
        getch();
        i++;
    }
    
    mvprintw(LINES - 3, 0, "System operational.");
    mvprintw(LINES - 1, 0, "I have read and agree to the terms of service (press space to confirm) ");
    
    nocbreak();
    if (get_keypress() != ' ')
    {
        mvprintw(LINES - 1, 0, "Are you sure you don't want to agree to the terms of service and exit ? (y/N) ");
        ch = get_keypress();
        if (ch == 'y' || ch == 'Y')
            return (1);
    }
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

char    *menu_port_select(void)
{
    int     i;
    int     ch;
    char    *port;
    char    **files_tab;

    ch = 0;
    files_tab = get_files_tab("/dev");
    while (1)
    {
        clear();
        refresh();
        
        mvprintw(0, 0, "[PORT SELECTION MENU]______________________________________________________________________\n");
        if (files_tab)
        {
            i = 0;
            while (files_tab[i])
            {
                printw(" [%02d]   %s\n", i + 1, files_tab[i]);
                i++;
            }
            printw("\n [R]    Refresh\n");
            printw(" [ESC]  Cancel\n");
            if (ch)
                mvprintw(LINES - 1, 0, ">> [ERROR] Invalid input, please try again: ");
            else
                mvprintw(LINES - 2, 0, "\nPlease type one of the above (1-%d | r-R | ESC) to proceed: ", i);
        }
        else
            break ;
        ch = get_keypress();
        if (ch >= '1' && ch <= i + '0' && files_tab[ch - '0' - 1])
        {
            port = files_tab[ch - '0' - 1];
            free(files_tab);
            return(port);
        }
        if (ch == 'r' || ch == 'R')
        {
            ch = 0;
            free(files_tab);
            files_tab = get_files_tab("/dev");
        }
        if (ch == '\e')
        {
            free(files_tab);
            break ;
        }
    }
    return(NULL);
}

int menu_baudrate_select(void)
{
    int ch;

    ch = 0;
    while (1)
    {
        clear();
        refresh();
        
        mvprintw(0, 0, "[BAUD RATE SELECTION MENU]_________________________________________________________________\n");
        printw("This is where you select the connection speed between *CONFEDERATION BOMB DEFUSER v4.6*\nand the bomb.\n");
        printw("The data/commands sent to the bomb have to be synchronized to its speed for it to be able\nto interpret them correctly.\n");
        printw("Please be aware that an incorrect synchronisation speed may send corrupted data/commands tothe bomb and trigger its defense mechanisms.\n\n");
        printw("For more informations on the topic of synchronizing communication speeds with a lethal bombplease read the fucking manual.\n\n");
        printw(" [01]   9600\n");
        printw(" [02]   42000\n");
        printw(" [03]   115200\n");
        printw("\n [ESC]  Cancel\n");
        if (ch)
            mvprintw(LINES - 1, 0, ">> [ERROR] Invalid input, please try again: ");
        else
            mvprintw(LINES - 2, 0, "\nPlease type one of the above (1-3 | ESC) to proceed: ");
        ch = get_keypress();
        if (ch >= '1' && ch <= '3')
            return (ch - '0');
        if (ch == 27)
            return (0);
    }
    return(0);
}

int menu_port_connect(char *port_name, int baudrate, struct termios *toptions)
{
    int     i;
    int     fd;
    int     delay;
    char    *loading_bar;
    char    *separation;
    
    i = 0;
    clear();
    loading_bar = "########################################";
    separation = "___________________________________________________________________________________________";
    while (i <= 60)
    {
        mvprintw(0, 0, "[BOMB DEFUSER] Establishing connection...\n\n");
        printw("Please DO NOT at any time shut down the defuser during this process.");

        // Open serial port
        if (i == 55)
        {
            fd = open(port_name, O_RDWR | O_NOCTTY);
            if (fd == -1) {
                endwin();
                perror("open");
                printf("%s\n", port_name);
                return (-1);
            }
            *toptions = set_termios_opt(fd, baudrate);
        }
        
        printw("\n%s\n\n\n", separation);
        printw("Opening port...");
        if (i >= 20)
            printw(" (OK)");
        printw("\nLOADING [%d%%][%.*s]\n", (i * 100 / 20) > 100 ? 100 : (i * 100 / 20), i * 2, loading_bar);
        
        if (i >= 20)
        {
            printw("\n\n\nDecoding encryption...");
            if (i >= 40)
                printw(" (OK)");
            printw("\nLOADING [%d%%][%.*s]\n", ((i - 20) * 100 / 20) > 100 ? 100 : ((i - 20) * 100 / 20), (i - 20) * 2, loading_bar);
        }
        
        if (i >= 40)
        {
            printw("\n\n\nAnalyzing firmware...");
            if (i == 60)
                printw(" (OK)");
            printw("\nLOADING [%d%%][%.*s]\n", (i - 40) * 100 / 20, (i - 40) * 2, loading_bar);
        }
        
        mvprintw(LINES - 4, 0, "%s\n", separation);
        printw("PROGRESS [%d%%][%.*s]\n", i * 100 / 60, i, "############################################################");
        
        if (i == 60)
            printw("Connecting to device.");
        
        delay = 100;
        if (i < 20 || i > 40)
            delay = 50;
        if (i == 60)
            delay = 150;
        
        cbreak();
        timeout(delay);
        getch();
        nocbreak();
        i++;
    }
    return (fd);
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
            *fd = menu_port_connect(g_port, cfgetispeed(toptions), toptions);
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
                *fd = menu_port_connect(g_port, cfgetispeed(toptions), toptions);
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

char    *print_output(int fd, char *last_out, int view)
{
    char    buf[255];
    char    *buf2;
    char    *temp;

    if (!fd)
        return (NULL);
    memset(buf, 0, sizeof(buf));
    buf2 = NULL;
    if (read(fd, buf, sizeof(buf)) > 0)
    {
        temp = buf2;
        buf2 = ft_strjoin(buf2, buf);
        free(temp);
    }
    if (view == 1)
        return (last_out);
    if (buf2 && buf2[strlen(buf2) - 3] == '$')
    {
        mvprintw(2, 0, "%s\n", crop(buf2));
        return (buf2);
    }
    if (last_out)
        mvprintw(2, 0, "%s\n", last_out);
    return (last_out);
}

char    *print_command_prompt(int *fd, char *curr_cmd, char *last_cmd, int *view, struct termios *toptions)
{
    int     ch;
    char    *cmd;
    
    if (curr_cmd)
        cmd = strdup(curr_cmd);
    else
        cmd = (char *) calloc(255, sizeof(char));
        
    if (last_cmd)
    {
        if (exec_command(fd, last_cmd, view, toptions))
            return (NULL);
    }
    
    if (!*fd)
    {
        mvprintw(LINES - 5, 0, "___________________________________________________________________________________________");
        printw("/!\\ Currently not connected to any device. /!\\\n");
        printw("Type 'help_connect' or read the manual (cmd 'man') for how to connect to a bomb.");
    }
    // Print command prompt
    mvprintw(LINES - 2, 0, "___________________________________________________________________________________________\n");
    printw("Command: %-0300s", cmd);
    move(LINES - 1, 9 + strlen(cmd));
    
    halfdelay(1);
    ch = getch();
    nocbreak();
    
    if (ch == '\n')
    {
        // Write to serial port
        if (cmd[0] == '@')
        {
            if (write(*fd, cmd + 1, strlen(cmd + 1)) == -1)
            {
                endwin();
                perror("write");
                exit(1);
            }
            memset(cmd, 0, strlen(cmd));
            if (*view == 1)
                *view = 2;
        }
        else if (cmd[0] != '@' && strlen(cmd) < 253)
        {
            cmd[strlen(cmd)] = ch;
            if (*view == 0)
                *view = 2;
        }
    }
    else if (ch == 127)
    {
        // Backspace
        if (strlen(cmd) > 0)
            cmd[strlen(cmd) - 1] = '\0';
    }
    else if (ch != ERR)
    {
        // Add character to command buffer
        if (strlen(cmd) < 253 && !((ch == ' ' || ch == '\t') && !strlen(cmd)))
            cmd[strlen(cmd)] = ch;
    }
    if (ch == '\t')
        *view = (*view + 1) % 3;
    return (cmd);
}

int menu_defusing(int *fd, struct termios *toptions)
{
    int     view;
    char    *cmd = NULL;
    char    *temp_cmd;
    char    *last_cmd = NULL;
    char    *out = NULL;
    char    *temp_out = NULL;

    view = 1;
    while (1) {
        clear();
        refresh();
        
        // Print the output of the bomb
        if (view == 0)
            mvprintw(0, 0, "[1 BOMB INTERPRETOR][2 ...]_________________________________(PORT %-15.15s @ %06d)", g_port, get_baudrate(toptions));
        if (view == 2)
            mvprintw(0, 0, "[1 BOMB INTERPRETOR]________________________________________(PORT %-15.15s @ %06d)", g_port, get_baudrate(toptions));
        if (out)
        {
            temp_out = crop(out);
            if (strstr(out, "SUPERUSER"))
                prompt = '#';
        }
        out = print_output(*fd, temp_out, view);
        if ((view == 0 || view == 2) && *fd)
        {
            printw("\nUSER ~ %c ", prompt);
            if (cmd && cmd[0] == '@')
                printw(cmd + 1);
            printw("\n");
        }
        
        // Print the debugger console and the output of the rpi
        if (view == 1)
            mvprintw(0, 0, "[1 ...][2 DEFUSER GUI]______________________________________(PORT %-15.15s @ %06d)\n", g_port, get_baudrate(toptions));
        if (view == 2)
            printw("______________________[2 DEFUSER GUI]______________________________________________________\n");
        temp_cmd = cmd;
        cmd = print_command_prompt(fd, cmd, last_cmd, &view, toptions);
        if (temp_cmd)
            free(temp_cmd);
        if (!cmd || (cmd && cmd[strlen(cmd) - 1] == '\n'))
        {
            if (last_cmd)
            {
                free(last_cmd);
                last_cmd = NULL;
            }
            if (cmd)
            {
                last_cmd = strdup(cmd);
                memset(cmd, 0, strlen(cmd));
            }
        }
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
