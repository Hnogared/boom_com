NAME		=	defuser_wizard

VPATH		=	Srcs:			\
				Srcs/init:		\
				Srcs/layouts:	\
				Srcs/builtins:	\
				Srcs/utils

SRCS		=	main.c					\
				init_data.c				\
				init_readline.c			\
				rlncurses.c				\
				check_cmds.c			\
		  		display_utils.c			\
		  		setup_functions.c		\
		  		strings_utils.c			\
		  		main_layout.c			\
		  		select_layouts.c		\
		  		loading_layouts.c		\
				layout_transitions.c	\
				errors_utils.c			\
				bit_stuffer.c

OBJS_DIR	=	Objs
OBJS		=	$(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

INCL		=	Includes

CC			=	gcc
CFLAGS		=	-Wall -Werror -Wextra -g
LIBS		=	-lncurses -lreadline

RM			=	rm -rf


all:		$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -I $(INCL) $(LIBS)

$(OBJS_DIR)/%.o:	%.c
	@if [ ! -d $(OBJS_DIR) ]; then	\
		mkdir $(OBJS_DIR);			\
		echo "mkdir $(OBJS_DIR)";	\
	fi
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INCL) $(LIBS)

clean:
	$(RM) $(OBJS)

fclean:		clean
	$(RM) $(NAME)

dclean:		clean
	$(RM) $(OBJS_DIR)

re:			fclean all

.PHONY: all clean fclean dclean re
