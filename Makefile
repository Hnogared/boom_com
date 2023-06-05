NAME		= defuser_wizard

SRCS_DIR	= Srcs
SRCS_MAIN	= main.c			\
		  	  loading_menus.c	\
		  	  select_menus.c	\
		  	  defusing_menu.c

UTILS_DIR	= Srcs/Utils
SRCS_UTILS	= check_cmds.c		\
		  	  display_utils.c	\
		  	  setup_functions.c	\
		  	  strings_utils.c	\

OBJS_DIR	= Objs
OBJS		= $(addprefix $(OBJS_DIR)/, $(SRCS_MAIN:.c=.o) $(SRCS_UTILS:.c=.o))

INCL		= Includes

CC			= gcc
CFLAGS		= -Wall -Werror -Wextra -lncurses

RM			= rm -rf


all:		$(NAME)

$(NAME):	$(OBJS)
	$(CC) $^ -o $@ -I $(INCL) $(CFLAGS)

$(OBJS_DIR)/%.o:	$(SRCS_DIR)/%.c
	@if [ ! -d $(OBJS_DIR) ]; then	\
		mkdir $(OBJS_DIR);			\
		echo "mkdir $(OBJS_DIR)";	\
	fi
	$(CC) -c $< -o $@ -I $(INCL) $(CFLAGS)

$(OBJS_DIR)/%.o:	$(UTILS_DIR)/%.c
	@if [ ! -d $(OBJS_DIR) ]; then	\
		mkdir $(OBJS_DIR);			\
		echo "mkdir $(OBJS_DIR)";	\
	fi
	$(CC) -c $< -o $@ -I $(INCL) $(CFLAGS)

clean:
	$(RM) $(OBJS)

fclean:		clean
	$(RM) $(NAME)

dclean:		clean
	$(RM) $(OBJS_DIR)

re:			fclean all

.PHONY: all clean fclean dclean re
