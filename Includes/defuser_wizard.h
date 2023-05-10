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
int chrstr(char c, char *str);
int left_strcmp(char *str1, char *str2);

char    *crop(char *str);
char    *ft_strjoin(char *dest, char *src);

void    free_str_tab(char **str_tab);

#endif
