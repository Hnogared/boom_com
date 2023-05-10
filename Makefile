NAME	= defuser_wizard

SRCSRAW	= defuser_wizard.c	\
		  strings_utils.c	\
		  loading_menus.c	\
		  select_menus.c
SRCSDIR	= Srcs
SRCS	= $(addprefix $(SRCSDIR)/, $(SRCSRAW))

OBJS	= $(addprefix $(OBJSDIR)/, $(SRCSRAW:.c=.o))
OBJSDIR	= Objs

INCL	= Includes

CC		= gcc
CFLAGS	= -Wall -Werror -Wextra -lncurses

RM		= rm -rf


all:		$(NAME)

$(NAME):	$(OBJS)
	$(CC) $^ -o $@ -I $(INCL) $(CFLAGS)

$(OBJSDIR)/%.o:	$(SRCSDIR)/%.c
	@if [ ! -d $(OBJSDIR) ]; then	\
		mkdir $(OBJSDIR);			\
		echo "mkdir $(OBJSDIR)";	\
	fi
	$(CC) -c $< -o $@ -I $(INCL) $(CFLAGS)

clean:
	$(RM) $(OBJS)

fclean:		clean
	$(RM) $(NAME)

dclean:		clean
	$(RM) $(OBJSDIR)

re:			fclean all

.PHONY: all clean fclean dclean re
