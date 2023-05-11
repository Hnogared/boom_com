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

typedef struct
{
	int				fd;
	char			*port;
	struct termios	*toptions;
} portopts;

extern char	*g_port;

/* strings_utils.c file */
int				chrstr(char c, char *str);
int				left_strcmp(char *str1, char *str2);
char			*crop(char *str);
char			*ft_strjoin(char *dest, char *src);
void			free_str_tab(char **str_tab);

/* display_utils.c file */
void	put_separation(int line);
void	put_loading(char *name, char *confirm, int line_pos, int progress, int length);

/* loading_menus.c file */
int				play_startup(void);
int				play_connect(char *port_name, int baudrate, struct termios *toptions);

/* select_menus.c file */
int				menu_baudrate_select(void);
char			*menu_port_select(void);

/* main_menu.c file */
int				exec_command(int *fd, char *command, int *view, struct termios *toptions);
int				menu_defusing(int *fd, struct termios *toptions);
char			*print_output(int fd, char *last_out, int view);
char			*print_prompt(int *fd, char *curr_cmd, char *last_cmd, int *view, struct termios *toptions);

/* check_cmds.c file */
int				check_view_cmds(char *command, int *view);
int				check_help_cmds(char *command, int *view);
int				check_conn_cmds(int *fd, char *command, int *view, struct termios *toptions);

/* setup_functions.c */
int				get_keypress(void);
int				get_baudrate(struct termios *toptions);
char			**get_files_tab(char *directory);
struct termios	set_termios_opt(int fd, int baudrate);
void			exit_helper(int fd, char *command);

#endif
