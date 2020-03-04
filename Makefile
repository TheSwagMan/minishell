# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/03/04 17:41:53 by tpotier           #+#    #+#              #
#    Updated: 2020/03/04 17:41:55 by tpotier          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
FILES = minishell
IFILES = minishell.h
NAME = minishell
SDIR = srcs/
IDIR = incs/
LIBFTDIR = libft/
LIBFT = $(LIBFTDIR)libft.a
LIBFTINC = $(LIBFTDIR)incs/
LIBFTFLAGS = -L$(LIBFTDIR) -lft
#LIBLSTDIR = liblst/
#LIBLST = $(LIBLSTDIR)liblst.a
#LIBLSTINC = $(LIBLSTDIR)incs/
#LIBLSTFLAGS = -L$(LIBLSTDIR) -llst
#CFLAGS = -Wall -Wextra -Werror -I$(IDIR) -I$(LIBFTINC) -I$(LIBLSTINC) -MMD -MP
CFLAGS = -Wall -Wextra -Werror -I$(IDIR) -I$(LIBFTINC) -MMD -MP

SRCS = $(addprefix $(SDIR), $(addsuffix .c, $(FILES)))
INCS = $(addprefix $(IDIR), $(IFILES))
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

all: $(NAME)

$(LIBFT):
	make -C $(LIBFTDIR)

$(LIBLST):
	make -C $(LIBLSTDIR)

$(NAME): $(OBJS) $(LIBFT) #$(LIBLST)
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LIBFTFLAGS) #$(LIBLSTFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	make -C $(LIBFTDIR) $@
	#make -C $(LIBLSTDIR) $@
	rm -f $(OBJS) $(DEPS)

fclean: clean
	make -C $(LIBFTDIR) $@
	#make -C $(LIBLSTDIR) $@
	rm -f $(NAME)

re: fclean all

.PHONY: all $(name) clean fclean re
-include $(DEPS)
