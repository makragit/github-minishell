# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/16 20:53:07 by mkrausho          #+#    #+#              #
#    Updated: 2024/12/11 18:18:57 by mkrausho         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# AR
# .a File: Static library archive file
# ar rc libft.a 
#	r	Insert th files member into archive
#	c	Create the archive
#	s	Write an object-file index to the archive ...
#	t	Display a table listing the contents of archive, or those of the files listed in member...

NAME = libft.a

SOURCE = ft_toupper.c \
		 ft_isalpha.c \
		 ft_isdigit.c \
		 ft_tolower.c \
		 ft_isalnum.c \
		 ft_isascii.c \
		 ft_isprint.c \
		 ft_strlen.c \
		 ft_strncmp.c \
		 ft_atoi.c \
		 ft_strchr.c \
		 ft_strrchr.c \
		 ft_bzero.c \
		 ft_strnstr.c \
		 ft_strlcpy.c \
		 ft_strdup.c \
		 ft_calloc.c \
		 ft_memset.c \
		 ft_memcpy.c \
		 ft_memmove.c \
		 ft_strlcat.c \
		 ft_memcmp.c \
		 ft_memchr.c \
		 ft_strjoin.c \
		 ft_split.c \
		 ft_itoa.c \
		 ft_strmapi.c \
		 ft_striteri.c \
		 ft_putchar_fd.c \
		 ft_putstr_fd.c \
		 ft_putendl_fd.c \
		 ft_putnbr_fd.c \
		 ft_substr.c \
		 ft_strtrim.c \
		 ./ft_printf/ft_printf.c \
		 ./ft_printf/fp_char.c \
		 ./ft_printf/fp_string.c \
		 ./ft_printf/fp_int.c \
		 ./ft_printf/fp_uint.c \
		 ./ft_printf/fp_lower_hex.c \
		 ./ft_printf/fp_upper_hex.c \
		 ./ft_printf/fp_ptr_hex.c \
		 ./ft_printf/fp_uint_putnbr.c \
		 ./ft_printf/fp_hex_putnbr.c \
		 ./ft_printf/sign_handler.c \
		 ./ft_printf/fp_psign.c \
		 ./ft_printf/ret_fp.c

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
