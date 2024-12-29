# MAKEFILE minishell

CC = cc
NAME = minishell
LIBFT = ./libft/libft.a
SOURCE = minishell.c \
		 ./mak/temp.c
CFLAGS = -Wall -Wextra -Werror -g # TODO -g
#CFLAGS = -Wall -Wextra -Werror

#FSAN = -fsanitize=address,undefined -fsanitize-memory-track-origins
FSAN = -O1 -fsanitize=address,undefined -fsanitize-memory-track-origins -fno-omit-frame-pointer

OBJECTS = $(SOURCE:.c=.o)
# LDFLAGS = -lpthreaood
LDFLAGS = -lreadline
DEBUG = ./debug/debug.a

RM = rm -f

all: $(NAME)

$(NAME): $(LIBFT) $(DEBUG) $(OBJECTS)
#		$(CC) $(OBJECTS) $(LIBFT) $(DEBUG) $(LDFLAGS) $(CFLAGS) $(FSAN) -o $(NAME)
		$(CC) $(OBJECTS) $(LIBFT) $(DEBUG) $(LDFLAGS) $(CFLAGS) -o $(NAME)
#		$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

clean:
	$(RM) $(OBJECTS)
	make clean -C ./libft/
#DEBUG
	make clean -C ./debug/

fclean: clean
	$(RM) $(NAME)
	make fclean -C ./libft/
#DEBUG
	make fclean -C ./debug/

re: fclean all

$(LIBFT): 
	make -C ./libft/

$(DEBUG):
	make -C ./debug/

run: $(NAME)
	./minishell

.PHONY: all clean fclean run
#.PHONY: all clean fclean
