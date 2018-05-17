CC	=	gcc

RM	=	rm -rf

MK	=	mkdir -p

CFLAGS	+=	-W -Wall -Werror
CFLAGS	+=	-I./include

LDFLAGS	=	-lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer

NAME	=	bomberman

SRCDIR    =	src
SRC	=	$(addprefix $(SRCDIR)/, $(SRCFILES))
SRCFILES	=	main.c \
	 			draw.c \
				init.c \
				input.c \
				map.c \
				player.c


OBJDIR	=	obj
OBJ	=	$(addprefix $(OBJDIR)/, $(notdir $(SRC:.c=.o)))

all:		$(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -fPIC

$(OBJDIR):
	$(MK) $@

$(NAME):	$(OBJDIR) $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS);

clean:
	$(RM) $(OBJDIR)

fclean:		clean
	$(RM) $(NAME)

re:		fclean all

.PHONY:		all clean fclean re