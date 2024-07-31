SRC =	src/interface.c \
	src/logic.c \
	src/main.c 

CFLAGS += -I./include

NAME = PreSuff

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