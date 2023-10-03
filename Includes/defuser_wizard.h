#ifndef DEFUSER_WIZARD_H
# define DEFUSER_WIZARD_H


// For strnlen() and wcwidth()
# define _GNU_SOURCE
# include <unistd.h>

# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
# include <stdnoreturn.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <termios.h>
# include <curses.h>
# include <ncurses.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <dirent.h>
# include <locale.h>

# include <wchar.h>
# include <wctype.h>

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

# define max(a, b)         \
  ({ typeof(a) _a = a;    \
     typeof(b) _b = b;    \
     _a > _b ? _a : _b; })

// Checks errors for (most) ncurses functions. CHECK(fn, x, y, z) is a checked
// version of fn(x, y, z).
# define CHECK(fn, ...)                            \
  do                                               \
      if (fn(__VA_ARGS__) == ERR)                  \
          fail_exit(#fn"("#__VA_ARGS__") failed"); \
  while (false)

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
	char	*cmd;
	char	cmd_output[BIG_BUFFER];
	char	bomb_output[BIG_BUFFER];
	WINDOW	*win;
	WINDOW	*msg_win;
	WINDOW	*cmd_win;
}				t_dispopts;

typedef struct s_rlncurses
{
	bool			visual_mode;
	bool			should_exit;
	bool			input_avail;
	unsigned char	input;
}				t_rlncurses;

typedef struct s_data
{
	t_portopts	portopts_s;
	t_dispopts	dispopts_s;
	t_rlncurses	rlncurses_s;
}				t_data;

extern t_data	g_data_s;


/* init.c */
void			init_display(t_rlncurses *rlncurses_p, t_dispopts *disp_options);
void			init_portopts(t_portopts *port_options);

/* init_readline.c */
void 			init_readline(void);

/* bit_stuffer.c */
void			bit_stuffer(t_portopts *portopts_p, t_dispopts *dispopts_p);

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
void			play_connect(t_portopts *portopts_p, t_dispopts *dispopts_p);

/* select_menus.c file */
int				menu_baudrate_select(void);
char			*menu_port_select(void);

/* defusing_menu.c file */
int				exec_command(t_portopts *portopts_p, t_dispopts *dispopts_p);
void			update_command(t_dispopts dispopts_s, t_rlncurses *rlncurses_p);
void			menu_defusing(WINDOW *msg_win, t_portopts *portopts_p, t_dispopts *dispopts_p);
void			cmd_win_redisplay(WINDOW *cmd_win, bool for_resize);
//void			print_output(t_portopts *port_options, t_dispopts *dispopts_p);
void			print_prompt(t_portopts *portopts_p);

/* check_cmds.c file */
int				check_view_cmds(t_dispopts *dispopts_p);
int				check_choice(WINDOW *win, t_portopts *portopts_p, t_dispopts *dispopts_p);
int				check_help_cmds(WINDOW *win, t_portopts *portopts_p, t_dispopts *dispopts_p);
int				check_conn_cmds(t_portopts *portopts_p, t_dispopts *dispopts_p);

/* setup_functions.c */
int				open_usb_port(t_portopts *portopts_p);
int				get_keypress(void);
int				get_baudrate(int mode);
char			**get_files_tab(char *directory);
struct termios	set_termios_opt(int fd, int baudrate);
void			exit_helper(t_portopts portopts_s, t_dispopts dispopts_s);

/* layout_changes.c */
void			goto_layout_help(WINDOW *win, t_portopts *portopts_p, t_dispopts *dispopts_p);
void			goto_layout_1(WINDOW *win, t_portopts *portopts_p, t_dispopts *dispopts_p);
void			goto_layout_2(WINDOW *win, t_portopts *portopts_p, t_dispopts *dispopts_p, bool loading);
void			goto_layout_3(WINDOW *win, t_portopts *portopts_p, t_dispopts *dispopts_p, bool loading);
void			goto_layout_firewalloff(WINDOW *win, t_portopts *portopts_p, t_dispopts *dispopts_p);
void			goto_layout_labyrinth(WINDOW *win, t_portopts *portopts_p, t_dispopts *dispopts_p);
void			goto_layout_bytes(WINDOW *win, t_portopts *portopts_p, t_dispopts *dispopts_p);
void			goto_layout_password(WINDOW *win, t_portopts *portopts_p, t_dispopts *dispopts_p, bool mode);

/* rlncurses.c */
size_t			strnwidth(const char *s, size_t n, size_t offset);
size_t			strwidth(const char *s, size_t offset);
noreturn void	fail_exit(const char *msg);
void 			resize(t_portopts *portopts_p, t_dispopts *dispopts_p);
void			forward_to_readline(char c, t_rlncurses *rlncurses_p);

#endif
