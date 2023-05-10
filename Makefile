NAME	= defuser_wizard

SRCSRAW	= defuser_wizard.c
SRCSDIR	= Srcs
SRCS	= $(addprefix $(SRCSDIR)/, $(SRCSRAW))

OBJS	= $(addprefix $(OBJSDIR)/, $(SRCSRAW:.c=.o))
OBJSDIR	= Objs

CC		= gcc
CFLAGS	= -Wall -Werror -Wextra -lncurses

RM		= rm -rf


all:		$(NAME)

$(NAME):	$(OBJS)
	$(CC) $^ -o $@ $(CFLAGS)

$(OBJSDIR)/%.o:	$(SRCSDIR)/%.c
	if [ ! -d $(OBJSDIR) ]; then	\
		mkdir $(OBJSDIR);			\
	fi
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	$(RM) $(OBJS)

fclean:		clean
	$(RM) $(NAME)

dclean:		clean
	$(RM) $(OBJSDIR)

re:			fclean all

.PHONY: all clean fclean dclean re
