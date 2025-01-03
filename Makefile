# MAKEFILE minishell

# CC = cc
CC = gcc

NAME = minishell
LIBFT = ./libft/libft.a


# Directories
UTILS_DIR = ./domagoj/custom_utilis
INPUT_CHCK = ./domagoj/parser/0_input_validation
TOKEN_DIR = ./domagoj/parser/1_tokenizer
TABLE_DIR = ./domagoj/parser/2_cmd_table
EXPANDER_DIR = ./domagoj/parser/3_expander


SOURCE = minishell.c \
		 ./mak/temp.c \
          $(TOKEN_DIR)/token.c \
          $(TOKEN_DIR)/token_make_entry.c \
          $(TOKEN_DIR)/token_len.c \
          $(TOKEN_DIR)/token_type.c \
          ./domagoj/parser/parse.c \
          $(UTILS_DIR)/lst_functions.c \
          $(UTILS_DIR)/ft_strexpel.c \
          $(UTILS_DIR)/ft_insert_str.c \
          $(UTILS_DIR)/ft_find_indexes.c \
		  $(UTILS_DIR)/print_or_free.c \
          $(UTILS_DIR)/ft_iswhitespace.c \
          $(TABLE_DIR)/fill_table.c \
          $(TABLE_DIR)/table_init.c \
		  $(TABLE_DIR)/create_arr.c \
		  $(EXPANDER_DIR)/expander.c \
		  $(EXPANDER_DIR)/del_quotes.c \
		  $(EXPANDER_DIR)/exit_status.c \
		  $(EXPANDER_DIR)/env_var.c \
		  $(INPUT_CHCK)/input_check.c \
		  ./domagoj/main-test.c \


CFLAGS = -Wall -Wextra -Werror -g # TODO -g


#FSAN = -fsanitize=address,undefined -fsanitize-memory-track-origins
FSAN = -O1 -fsanitize=address,undefined -fsanitize-memory-track-origins -fno-omit-frame-pointer

OBJECTS = $(SOURCE:.c=.o)

# LDFLAGS = -lreadline
LDFLAGS = -lreadline  -lncurses # removed: -lft -Llibft-delete

DEBUG = ./debug/debug.a

RM = rm -f

all: $(NAME)

$(NAME): $(LIBFT) $(DEBUG) $(OBJECTS)
#		$(CC) $(OBJECTS) $(LIBFT) $(DEBUG) $(LDFLAGS) $(CFLAGS) $(FSAN) -o $(NAME)
		$(CC) $(OBJECTS) $(LIBFT) $(DEBUG) $(LDFLAGS) $(CFLAGS) -o $(NAME)
#		$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

# Compile source files into object files
#%.o: %.c
#	$(CC) $(CFLAGS) -c $< -o $@

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

# Build and clean up after
exe: re
	$(MAKE) clean

.PHONY: all clean fclean run re exe 
#.PHONY: all clean fclean
