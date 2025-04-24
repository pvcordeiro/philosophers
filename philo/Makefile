NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC =	src/main.c \
		src/actions.c \
		src/creation.c \
		src/helpers.c \
		src/printer.c \
		src/singleton.c \
		src/threads.c \
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
        src/utils/utils11.c
INCLUDES = -I headers
OBJ_DIR = obj
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
LEAK = valgrind --leak-check=full --show-leak-kinds=all \
	--trace-children=yes --track-origins=yes -s
HELL = valgrind --tool=helgrind

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

test: all
	@./$(NAME) 10 800 100 200 10

e: all
	@./$(NAME) 10 800 200 200 10 > result.txt && grep eating result.txt | awk '{print $2}' | sort | uniq -c; rm result.txt

val:
	$(LEAK) ./$(NAME) 2 300 200 200

hel:
	$(HELL) ./$(NAME) 5 8100 2000 2000 7