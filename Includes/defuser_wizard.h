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


/* ************************************************************************** */
/* defines                                                                    */
/* ************************************************************************** */
# ifndef BIN_NAME
#  define BIN_NAME	"defuser_wizard"
# endif

# define FILES_TAB_SIZE	9
# define LITTLE_BUFFER	80
# define BIG_BUFFER		4096
# define BOMBOUT_BUFFER	4096

# define BOMB_VIEW		0
# define DEFUSER_VIEW	1
# define SPLIT_VIEW		2

# define PROMPT	"commande %>"

# define KEYSEQ_SHIFT_UP	"\e[1;2A"
# define KEYSEQ_SHIFT_DOWN	"\e[1;2B"

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


/* ************************************************************************** */
/* structures declarations                                                    */
/* ************************************************************************** */
typedef struct s_history
{
	char	*cmd;
	char	*bomb_output;
	void	*previous;
	void	*next;
}				t_history;

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
	t_history	*bomb_out_history;
}				t_data;

extern t_data	g_data_s;


/* ************************************************************************** */
/* prototypes                                                                 */
/* ************************************************************************** */

/* Srcs/bomb_communication ************************************************** */
/* bomb_read_write.c */
void			read_bomb_out(t_portopts *portopts_p, t_dispopts *dispopts_p);

/* bomb_out_history.c */
t_history		*new_history_node(char *cmd, char *bomb_out, void *previous,
	void *next);
t_history		*history_add_front(t_history **history_p, t_history *new_node);
void			free_history_node(t_history *node_p);
void			free_history(t_history *history_p);


/* Srcs/builtins ************************************************************ */
/* bit_stuffer.c */
void			bit_stuffer(t_portopts *portopts_p, t_dispopts *dispopts_p);


/* Srcs/cmd_win ************************************************************* */
/* check_cmds.c */
int				check_view_cmds(t_dispopts *dispopts_p);
int				check_choice(t_portopts *portopts_p, t_dispopts *dispopts_p);
int				check_help_cmds(t_portopts *portopts_p, t_dispopts *dispopts_p);
int				check_conn_cmds(t_portopts *portopts_p, t_dispopts *dispopts_p);

/* cmd_win_display.c */
void 			resize(t_portopts *portopts_p, t_dispopts *dispopts_p);
void			print_cmd_win(WINDOW *cmd_win, bool for_resize);


/* Srcs/init **************************************************************** */
/* init_data.c */
void			init_display(t_rlncurses *rlncurses_p, t_dispopts *disp_options);
void			init_portopts(t_portopts *port_options);

/* init_readline.c */
void 			init_readline(void);

/* command_handling.c */
int				get_keypress(void);
int				exec_command(t_data *data_p);
void			update_command(t_portopts *portopts_p, t_dispopts *dispopts_s, t_rlncurses *rlncurses_p);


/* Srcs/utils *************************************************************** */
/* display_utils.c file */
void			main_menu_loading(char *message);
void			put_centered(char *str, int line, int width);
void			put_separation(int line, int width);
void			put_loading(char *name, char *confirm, int line_pos, int progress, int length);

/* errors_utils.c */
noreturn void	fail_exit(const char *msg);
void			save_error(char *dest, size_t dest_buff, const char *prefix,
	const char *prefix2);

/* setup_functions.c */
int				open_usb_port(t_portopts *portopts_p);
int				get_baudrate(int mode);
char			**get_files_tab(char *directory);
struct termios	set_termios_opt(int fd, int baudrate);
void			exit_helper(t_data data_s);

/* strings_utils.c */
char			*safer_strncpy(char *dest, char *src, size_t dest_len, size_t len);
char			*ft_strtrim(char *str, char *set);
char			*crop(char *str);
char			*ft_strjoin(char *dest, char *src);
void			free_str_tab(char **str_tab);

/* strings_utils_2.c */
size_t			strnwidth(const char *s, size_t n, size_t offset);
size_t			strwidth(const char *s, size_t offset);


/* Srcs/layouts ************************************************************* */
/* layout_transitions.c */
void			goto_layout_help(t_portopts *portopts_p, t_dispopts *dispopts_p);
void			goto_layout_1(t_portopts *portopts_p, t_dispopts *dispopts_p);
void			goto_layout_2(t_portopts *portopts_p, t_dispopts *dispopts_p, bool loading);
void			goto_layout_3(t_portopts *portopts_p, t_dispopts *dispopts_p, bool loading);
void			goto_layout_firewalloff(t_portopts *portopts_p, t_dispopts *dispopts_p);
void			goto_layout_labyrinth(t_portopts *portopts_p, t_dispopts *dispopts_p);
void			goto_layout_bytes(t_portopts *portopts_p, t_dispopts *dispopts_p);
void			goto_layout_password(t_portopts *portopts_p, t_dispopts *dispopts_p, bool mode);

/* main_layout.c */
void			main_layout(t_portopts *portopts_p, t_dispopts *dispopts_p);

/* loading_layouts.c */
int				play_startup(void);
void			play_connect(t_portopts *portopts_p, t_dispopts *dispopts_p);

/* select_layouts.c */
int				menu_baudrate_select(void);
char			*menu_port_select(void);

#endif
