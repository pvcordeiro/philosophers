NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c \
		src/utils/utils1.c \
		src/utils/utils2.c \
		src/utils/utils3.c \
		src/utils/utils4.c \
		src/utils/utils5.c \
		src/utils/utils6.c \
		src/utils/utils7.c \
		src/utils/utils8.c \
		src/utils/utils9.c \
		src/utils/utils10.c \
		src/utils/utils11.c \
		src/ft_printf.c
INCLUDES = -I header
OBJ = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJ)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

test: all
	@./$(NAME) 5 800 200 200