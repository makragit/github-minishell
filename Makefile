# MAKEFILE minishell

# CC = cc
CC = gcc
# CC = clang

NAME = minishell
LIBFT = ./libft/libft.a


# Directories
# UTILS_DIR = ./custom_utilis
# INPUT_CHCK = ./parser/lexer
# TOKEN_DIR = ./parser/tokenisation
# TABLE_DIR = ./parser/table_fill
# EXPANDER_DIR = ./parser/expansion
# HEREDOC_DIR = ./execution/heredoc_program
# PIPE_DIR = ./execution/pipe_redir

UTILS_DIR = ./domagoj/custom_utilis
INPUT_CHCK = ./domagoj/parser/lexer
TOKEN_DIR = ./domagoj/parser/tokenisation
TABLE_DIR = ./domagoj/parser/table_fill
EXPANDER_DIR = ./domagoj/parser/expansion
HEREDOC_DIR = ./domagoj/execution/heredoc_program
PIPE_DIR = ./domagoj/execution/pipe_redir

SOURCE = minishell.c \
		 ./mak/builtin_cd.c \
		 ./mak/builtin_export.c \
		 ./mak/builtin_general.c \
		 ./mak/error.c \
		 ./mak/free.c \
		 ./mak/key_handling.c \
		 ./mak/misc.c \
		 ./mak/prompt.c \
		 ./mak/signals.c \
		 ./mak/struct.c \
		 ./mak/utils_array.c \
		 ./mak/utils_array_2.c \
          $(TOKEN_DIR)/token.c \
          $(TOKEN_DIR)/token_make_entry.c \
          $(TOKEN_DIR)/token_len.c \
          $(TOKEN_DIR)/token_type.c \
          ./domagoj/parser/parse.c \
          $(UTILS_DIR)/lst_functions.c \
		  $(UTILS_DIR)/cmd_table_funcs.c \
          $(UTILS_DIR)/ft_strexpel.c \
		  $(INPUT_CHCK)/print_error.c \
          $(UTILS_DIR)/ft_insert_str.c \
          $(UTILS_DIR)/ft_find_indexes.c \
		  $(UTILS_DIR)/skip_s_quote.c \
		  $(UTILS_DIR)/ft_append.c \
		  $(UTILS_DIR)/ft_create_file.c \
		  $(UTILS_DIR)/read_line.c \
		  $(UTILS_DIR)/print_or_free.c \
		  $(UTILS_DIR)/getenv_local.c \
          $(UTILS_DIR)/ft_iswhitespace.c \
          $(TABLE_DIR)/fill_table.c \
          $(TABLE_DIR)/table_init.c \
		  $(TABLE_DIR)/create_arr.c \
		  $(TABLE_DIR)/create_redir.c \
		  $(EXPANDER_DIR)/expander.c \
		  $(EXPANDER_DIR)/del_quotes.c \
		  $(EXPANDER_DIR)/env_var.c \
		  $(INPUT_CHCK)/input_check.c \
		  $(HEREDOC_DIR)/heredoc_program.c \
		  $(PIPE_DIR)/pipe_redir.c \
		 ./domagoj/execution/execute.c \
		 ./domagoj/execution/test_builtin.c \
		 ./domagoj/execution/cmd_path.c \
		 ./domagoj/execution/redirect.c \
		 ./domagoj/execution/child_execution.c \
		 $(UTILS_DIR)/ft_free_array.c \
		 ./42-Get-next-line/get_next_line.c \
		 ./mak/DEL_tester.c						

CFLAGS = -Wall -Wextra -Werror -g # TODO -g

#FSAN = -fsanitize=address,undefined -fsanitize-memory-track-origins
FSAN = -O1 -fsanitize=address,undefined -fsanitize-memory-track-origins -fno-omit-frame-pointer

OBJECTS = $(SOURCE:.c=.o)

# LDFLAGS = -lreadline
LDFLAGS = -lreadline  -lncurses # removed: -lft -Llibft-delete

# DEBUG = ./debug/debug.a

RM = rm -f

all: $(NAME)

$(NAME): $(LIBFT) $(DEBUG) $(OBJECTS)
		$(CC) $(OBJECTS) $(LIBFT) $(LDFLAGS) $(CFLAGS) -o $(NAME)
#		$(CC) $(OBJECTS) $(LIBFT) $(LDFLAGS) $(CFLAGS) $(FSAN) -o $(NAME)



#		$(CC) $(OBJECTS) $(LIBFT) $(DEBUG) $(LDFLAGS) $(CFLAGS) -o $(NAME)

#		$(CC) $(OBJECTS) $(LIBFT) $(DEBUG) $(LDFLAGS) $(CFLAGS) $(FSAN) -o $(NAME)
#		$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

# Compile source files into object files
#%.o: %.c
#	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS)
	make clean -C ./libft/
#DEBUG
	# make clean -C ./debug/

fclean: clean
	$(RM) $(NAME)
	make fclean -C ./libft/
#DEBUG
	# make fclean -C ./debug/

re: fclean all

$(LIBFT):
	make -C ./libft/

# $(DEBUG):
	# make -C ./debug/

run: $(NAME)
	./minishell

# Build and clean up after
exe: re
	$(MAKE) clean

.PHONY: all clean fclean run re exe
#.PHONY: all clean fclean
