SRC = src/file.c src/response.c src/server.c src/utils.c

CFLAGS += -I./include

NAME = minserver

RM = rm -f

CP = cp -f

DESTDIR = /usr/bin/

SRCMAN = man/

MANPAGE = minserver.1

COMPMAN = minserver.1.gz 

COMPRESS = gzip < $(SRCMAN)$(MANPAGE) > $(SRCMAN)$(COMPMAN)

MANDIR = /usr/share/man/man1/

MANDB = mandb

CC = gcc

OBJS=	$(SRC:.c=.o)

LDFLAGS = -lssl

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -g -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

cleanMan:
	$(RM) $(SRCMAN)$(COMPMAN)

fclean: clean cleanMan
	$(RM) $(NAME)

install: $(NAME) 
	$(CP) $(NAME) $(DESTDIR)
	$(COMPRESS)
	$(CP) $(SRCMAN)$(COMPMAN) $(MANDIR)
	$(MANDB)

re: fclean all

uninstall: $(NAME)
	$(RM) $(DESTDIR)$(NAME)
	$(RM) $(MANDIR)$(COMPMAN)
	$(MANDB)

.PHONY: all clean fclean install re uninstall