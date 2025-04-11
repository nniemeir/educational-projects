SRC = src/input.c \
	src/interface.c \
	src/inventory.c \
	src/logic.c \
	src/main.c \
	src/stats.c

CFLAGS += -I./include

NAME = Halfway_Across

RM = rm -f

CP = cp -f

CC = gcc

OBJS=	$(SRC:.c=.o)

CFLAGS = -Wall -Wextra -pedantic -g

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -g -o $(NAME) $(OBJS) $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean 
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
