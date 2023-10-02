// copyright (c) 2015-2019, ulf magnusson
// spdx-license-identifier: isc

#ifndef RLNCURSES_H
# define RLNCURSES_H

// For strnlen() and wcwidth()
# define _XOPEN_SOURCE 700

# include <curses.h>
# include <locale.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdnoreturn.h>
# include <string.h>
# include <wchar.h>
# include <wctype.h>

#define max(a, b)         \
  ({ typeof(a) _a = a;    \
     typeof(b) _b = b;    \
     _a > _b ? _a : _b; })

// Checks errors for (most) ncurses functions. CHECK(fn, x, y, z) is a checked
// version of fn(x, y, z).
#define CHECK(fn, ...)                             \
  do                                               \
      if (fn(__VA_ARGS__) == ERR)                  \
          fail_exit(#fn"("#__VA_ARGS__") failed"); \
  while (false)

typedef struct s_rlncurses
{
	bool			visual_mode;
	bool			should_exit;
	bool			input_avail;
	unsigned char	input;
}				t_rlncurses;

#endif
