# DEBUG. TEST
# DEBUG. TEST
# DEBUG. TEST

NAME = debug.a

SOURCE = debug.c

OBJECTS = $(SOURCE:.c=.o)
CFLAGS = -Wall -Wextra -Werror

RM = rm -f

all: $(NAME)

$(NAME): $(OBJECTS)
	ar rcs $(NAME) $(OBJECTS)

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re