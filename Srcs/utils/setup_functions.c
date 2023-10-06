#include "../Includes/defuser_wizard.h"

int	open_usb_port(t_portopts *portopts_p)
{
	char	**usb_paths;

	usb_paths = (char *[]){"/dev/ttyUSB0", "/dev/ttyUSB1", "/dev/ttyUSB2",
		"/dev/ttyUSB3", "/dev/ttyUSB4", NULL};
	portopts_p->fd = -1;
	while (*usb_paths && portopts_p->fd == -1)
		portopts_p->fd = open(*(usb_paths++), O_RDWR | O_NOCTTY);
	if (portopts_p->fd == -1)
		return (1);
	memmove(portopts_p->port, *usb_paths, 13);
	portopts_p->baudrate = 3;
	set_termios_opt(portopts_p->fd, 3);
	return (0);
}

int	get_baudrate(int mode)
{
	if (mode == 0 || mode == 1)
		return (9600);
	if (mode == 2)
		return (42000);
	if (mode == 3)
		return (115200);
	return (0);
}

char	**get_files_tab(char *directory)
{
	int				i;
	char			**files_tab;
	DIR				*d;
	struct dirent	*dir;

	d = opendir(directory);
	if (!d)
		return (NULL);
	i = 0;
	files_tab = (char **) malloc((FILES_TAB_SIZE + 1) * sizeof(char *));
	dir = readdir(d);
	while (dir && i < FILES_TAB_SIZE)
	{
		if (!strncmp(dir->d_name, "tty", 3) && dir->d_name[3]
			&& !(dir->d_name[3] >= '0' && dir->d_name[3] <= '9'))
			files_tab[i++] = dir->d_name;
		dir = readdir(d);
	}
	files_tab[i] = NULL;
	closedir(d);
	return (files_tab);
}

struct termios	set_termios_opt(int fd, int baudrate)
{
	speed_t			to_set;
	struct termios	toptions;

	to_set = B9600 * (baudrate == 0 || baudrate == 1)
		+ B38400 * (baudrate == 2)
		+ B115200 * (baudrate < 0 || baudrate > 2);
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
	usleep(1000 * 1000);
	tcflush(fd, TCIFLUSH);
	return (toptions);
}

static void	free_dispopts(t_dispopts dispopts_s)
{
	if (dispopts_s.win)
		CHECK(delwin, dispopts_s.win);
	if (dispopts_s.cmd_win)
		CHECK(delwin, dispopts_s.cmd_win);
	CHECK(endwin);
}

void	exit_helper(t_data data_s)
{
	clear();
	attron(COLOR_PAIR(1));
	mvprintw(0, 0, "Exiting bomb defusing helper, goodbye...");
	attroff(COLOR_PAIR(1));
	if (data_s.portopts_s.fd > -1)
		close(data_s.portopts_s.fd);
	free_dispopts(data_s.dispopts_s);
	if (data_s.bomb_out_history)
		free_history(data_s.bomb_out_history);
	rl_clear_history();
	rl_callback_handler_remove();
	rl_unbind_key('\t');
	curs_set(1);
	exit(0);
}
