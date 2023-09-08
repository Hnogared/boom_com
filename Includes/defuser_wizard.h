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
# include <errno.h>

# define FILES_TAB_SIZE	9
# define LITTLE_BUFFER	80
# define BIG_BUFFER		4096

	/*
	 *	COLOR_BLACK   0
     *  COLOR_RED     1
     *  COLOR_GREEN   2
     *  COLOR_YELLOW  3
     *  COLOR_BLUE    4
     *  COLOR_MAGENTA 5
     *  COLOR_CYAN    6
     *	COLOR_WHITE   7
	 */
# ifndef COLOR_BG
#  define COLOR_BG		0
#  undef COLOR_BLACK
#  define COLOR_BLACK	8
# endif

typedef struct	portopts
{
	int				fd;
	int				baudrate;
	char			port[LITTLE_BUFFER];
	struct termios	*toptions;
}				portopts;

typedef struct	dispopts
{
	int		view;
	int		stage;
	int		cmd_len;
	char	prompt_char;
	char	cmd[LITTLE_BUFFER];
	char	cmd_output[BIG_BUFFER];
	char	bomb_output[BIG_BUFFER];
	WINDOW	*win;
}				dispopts;

/* strings_utils.c file */
int				chrstr(char c, char *str);
int				left_strcmp(char *str1, char *str2);
char			*crop(char *str);
char			*ft_strjoin(char *dest, char *src);
void			free_str_tab(char **str_tab);

/* display_utils.c file */
void			put_centered(char *str, int line, int width);
void			put_separation(int line, int width);
void			put_loading(char *name, char *confirm, int line_pos, int progress, int length);

/* loading_menus.c file */
int				play_startup(void);
void			play_connect(portopts **conn_options, dispopts **disp_options);
void			play_accessing_device(void);

/* select_menus.c file */
int				menu_baudrate_select(void);
char			*menu_port_select(void);

/* defusing_menu.c file */
int				exec_command(portopts **conn_options, dispopts **disp_options);
int				menu_defusing(portopts **conn_options, dispopts **disp_options);
void			print_output(portopts *conn_options, dispopts **disp_options);
void			print_prompt(portopts **conn_options, dispopts **disp_options);

/* check_cmds.c file */
int				check_view_cmds(dispopts **disp_options);
int				check_choice(portopts **conn_options, dispopts **disp_options);
int				check_help_cmds(dispopts **disp_options);
int				check_conn_cmds(portopts **conn_options, dispopts **disp_options);

/* setup_functions.c */
int				get_keypress(void);
int				get_baudrate(struct termios *toptions);
char			**get_files_tab(char *directory);
struct termios	set_termios_opt(int fd, int baudrate);
void			exit_helper(portopts *conn_options, dispopts *disp_options);

#endif
