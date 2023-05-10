#ifndef DEFUSER_WIZARD_H
# define DEFUSER_WIZARD_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <termios.h>
# include <unistd.h>
# include <ncurses.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <dirent.h>

/* strings_utils.c file */
int		chrstr(char c, char *str);
int		left_strcmp(char *str1, char *str2);

char	*crop(char *str);
char	*ft_strjoin(char *dest, char *src);

void	free_str_tab(char **str_tab);

/* loading_menus.c file */
int		play_startup(void);
int		play_connect(char *port_name, int baudrate, struct termios *toptions);

/* select_menus.c file */
int		menu_baudrate_select(void);

char    *menu_port_select(void);

/* defusing_menu.c file */
int		menu_defusing(int *fd, struct termios *toptions);
char    *print_output(int fd, char *last_out, int view);
char    *print_prompt(int *fd, char *curr_cmd, char *last_cmd, int *view, struct termios *toptions);

/* check_cmds.c file */
int check_help_cmds(char *command, int *view);

#endif
