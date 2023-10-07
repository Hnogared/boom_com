#ifndef DEFUSER_WIZARD_MACROS_H
# define DEFUSER_WIZARD_MACROS_H

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

# define max(a, b)        \
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

#endif
