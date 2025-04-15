NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC =	src/main.c \
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
        src/utils/ft_printf.c
INCLUDES = -I headers
OBJ_DIR = obj
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJ) -pthread

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm $(NAME)

re: fclean all

test: re
	@./$(NAME) 5 800 200 200 100