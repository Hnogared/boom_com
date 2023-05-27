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
	int				baudrate;
	char			*port;
	struct termios	*toptions;
}	portopts;

extern char	*g_port;

/* strings_utils.c file */
int				chrstr(char c, char *str);
int				left_strcmp(char *str1, char *str2);
char			*crop(char *str);
char			*ft_strjoin(char *dest, char *src);
void			free_str_tab(char **str_tab);

/* display_utils.c file */
void			put_separation(int line);
void			put_loading(char *name, char *confirm, int line_pos, int progress, int length);

/* loading_menus.c file */
int				play_startup(void);
void			play_connect(char *port_name, portopts *conn_options);

/* select_menus.c file */
int				menu_baudrate_select(void);
char			*menu_port_select(void);

/* main_menu.c file */
int				exec_command(portopts *conn_options, char *command, int *view);
int				menu_defusing(portopts *conn_options);
char			*print_output(int fd, char *last_out, int view);
char			*print_prompt(portopts *conn_options, char *curr_cmd, char *last_cmd, int *view);

/* check_cmds.c file */
int				check_view_cmds(char *command, int *view);
int				check_help_cmds(char *command, int *view);
int				check_conn_cmds(portopts *conn_options, char *command, int *view);

/* setup_functions.c */
int				get_keypress(void);
int				get_baudrate(struct termios *toptions);
char			**get_files_tab(char *directory);
struct termios	set_termios_opt(int fd, int baudrate);
void			exit_helper(int fd, char *command);

#endif
