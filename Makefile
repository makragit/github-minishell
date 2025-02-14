# MAKEFILE minishell

CC = cc

NAME = minishell
LIBFT = ./libft/libft.a

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
		 ./mak/builtin_unset.c \
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
		 ./mak/new.c \
          $(TOKEN_DIR)/token.c \
          $(TOKEN_DIR)/token_make_entry.c \
          $(TOKEN_DIR)/token_len.c \
          $(TOKEN_DIR)/token_type.c \
          $(UTILS_DIR)/compress_str.c \
		  $(UTILS_DIR)/var_name.c \
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
		  $(UTILS_DIR)/replace.c \
		  $(HEREDOC_DIR)/heredoc_program.c \
		  $(PIPE_DIR)/pipe_redir.c \
		 ./domagoj/execution/execute.c \
		 ./domagoj/execution/test_builtin.c \
		 ./domagoj/execution/cmd_path.c \
		 ./domagoj/execution/redirect.c \
		 ./domagoj/execution/child_execution.c \
		 $(UTILS_DIR)/ft_free_array.c \
		 ./42-Get-next-line/get_next_line.c \
		 ./DEL_tester.c \

# TODO DELETE DEL_tester

CFLAGS = -Wall -Wextra -Werror -g # TODO
# CFLAGS = -Wall -Wextra -Werror

OBJECTS = $(SOURCE:.c=.o)

LDFLAGS = -lreadline  -lncurses

RM = rm -f

all: $(NAME)

$(NAME): $(LIBFT) $(DEBUG) $(OBJECTS)
		$(CC) $(OBJECTS) $(LIBFT) $(LDFLAGS) $(CFLAGS) -o $(NAME)

clean:
	$(RM) $(OBJECTS)
	make clean -C ./libft/

fclean: clean
	$(RM) $(NAME)
	make fclean -C ./libft/

re: fclean all

$(LIBFT):
	make -C ./libft/


run: $(NAME)
	./minishell

exe: re
	$(MAKE) clean

.PHONY: all clean fclean run re exe
