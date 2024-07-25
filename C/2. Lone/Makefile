SRC =	src/interface.c \
	src/inventory.c \
	src/main.c \
	src/scenarios.c \
	src/stats.c

CFLAGS += -I./include

NAME = LONE

RM = rm -f

CC = gcc

OBJS=	$(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re