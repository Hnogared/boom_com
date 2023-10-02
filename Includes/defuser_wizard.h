#ifndef DEFUSER_WIZARD_H
# define DEFUSER_WIZARD_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <stdbool.h>
# include <termios.h>
# include <ncurses.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <dirent.h>

# include "rlncurses.h"

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

typedef struct s_portopts
{
	int				fd;
	int				baudrate;
	char			port[LITTLE_BUFFER];
	struct termios	*toptions;
}				t_portopts;

typedef struct s_dispopts
{
	int		layout;
	int		view;
	int		cmd_len;
	char	prompt_char;
	char	cmd[LITTLE_BUFFER];
	char	cmd_output[BIG_BUFFER];
	char	bomb_output[BIG_BUFFER];
	WINDOW	*win;
}				t_dispopts;

typedef struct s_data
{
	t_portopts	portopts_s;
	t_dispopts	dispopts_s;
	t_rlncurses	rlncurses_s;
}				t_data;

extern t_data	g_data_s;


/* init.c */
void			init_display(t_dispopts *disp_options);
void			init_portopts(t_portopts *port_options);
void 			init_readline(void);

/* bit_stuffer.c */
void			bit_stuffer(t_portopts **port_options, t_dispopts **disp_options);

/* strings_utils.c file */
int				chrstr(char c, char *str);
int				left_strcmp(char *str1, char *str2);
char			*crop(char *str);
char			*ft_strjoin(char *dest, char *src);
void			free_str_tab(char **str_tab);

/* display_utils.c file */
void			main_menu_loading(char *message);
void			put_centered(char *str, int line, int width);
void			put_separation(int line, int width);
void			put_loading(char *name, char *confirm, int line_pos, int progress, int length);

/* loading_menus.c file */
int				play_startup(void);
void			play_connect(t_portopts **port_options, t_dispopts **disp_options);

/* select_menus.c file */
int				menu_baudrate_select(void);
char			*menu_port_select(void);

/* defusing_menu.c file */
int				exec_command(t_portopts **port_options, t_dispopts **disp_options);
void			update_command(t_portopts **port_options, t_dispopts **disp_options);
void			menu_defusing(t_portopts **port_options, t_dispopts **disp_options);
void			print_output(t_portopts *port_options, t_dispopts **disp_options);
void			print_prompt(t_portopts **port_options, t_dispopts **disp_options);

/* check_cmds.c file */
int				check_view_cmds(t_dispopts **disp_options);
int				check_choice(t_portopts **port_options, t_dispopts **disp_options);
int				check_help_cmds(t_portopts **port_options, t_dispopts **disp_options);
int				check_conn_cmds(t_portopts **port_options, t_dispopts **disp_options);

/* setup_functions.c */
int				open_usb_port(t_portopts **port_options);
int				get_keypress(FILE *foo);
//int				get_keypress(void);
int				get_baudrate(int mode);
char			**get_files_tab(char *directory);
struct termios	set_termios_opt(int fd, int baudrate);
void			exit_helper(t_portopts *port_options, t_dispopts *disp_options);

/* layout_changes.c */
void			goto_layout_help(t_portopts **port_options, t_dispopts **disp_options);
void			goto_layout_1(t_portopts **port_options, t_dispopts **disp_options);
void			goto_layout_2(t_portopts **port_options, t_dispopts **disp_options, bool loading);
void			goto_layout_3(t_portopts **port_options, t_dispopts **disp_options, bool loading);
void			goto_layout_firewalloff(t_portopts **port_options, t_dispopts **disp_options);
void			goto_layout_labyrinth(t_portopts **port_options, t_dispopts **disp_options);
void			goto_layout_bytes(t_portopts **port_options, t_dispopts **disp_options);
void			goto_layout_password(t_portopts **port_options, t_dispopts **disp_options, bool mode);

#endif
