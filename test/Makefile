#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mcanal <mcanal@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/11/29 13:16:03 by mcanal            #+#    #+#              #
#    Updated: 2017/04/12 12:01:37 by mc               ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME =		wolfytest

C_NAME =	main.c

C_MAZE =	test_generate_maze.c

C_RAYC =	test_get_wall_coord.c

C_SDL =		test_move_player.c

O_DIR =		obj
VPATH =		src				src/maze		src/raycaster	src/sdl
SRCS =		$(C_NAME)		$(C_MAZE)		$(C_RAYC)		$(C_SDL)

OBJS =		$(SRCS:%.c=$(O_DIR)/%.o)
TEST_OBJS =	$(shell find ../obj -name \*.o | grep -v main) #TODO
DEPS =		$(OBJS:%.o=%.d)

LFT_DIR =	../libft
LFT_I_DIR = -I$(LFT_DIR)/inc
LFT =		$(LFT_DIR)/libft.a
LFT_LIB =	-L$(LFT_DIR) -lft

MIN_DIR =	$(LFT_DIR)/test/minunit
MIN_I_DIR = -I$(MIN_DIR)
MIN =		$(MIN_DIR)/minunit.h
MIN_LIB =

SDL_DIR = ../SDL
SDL_INSTALL_DIR = $(PWD)/$(SDL_DIR)/install
SDL_I_DIR = `./$(SDL_DIR)/sdl2-config --cflags`
SDL_LIB =	`./$(SDL_DIR)/sdl2-config --libs`
SDL =	$(SDL_DIR)/build/libSDL2.la

I_DIR =		$(LFT_I_DIR)	$(MIN_I_DIR)	$(SDL_I_DIR)	-Iinc	-I../inc
LIBS =		$(LFT_LIB)		$(MIN_LIB)		$(SDL_LIB)		-lm

RM =		rm -rf
MKDIR =		mkdir -p
MAKE =		make
MAKEFLAGS = -j 4
ECHO =		echo -e
CC =		$(shell clang --version >/dev/null 2>&1 && echo clang || echo gcc)
CFLAGS =	-Wall -Wextra -Werror -O2

UNAME_S =   $(shell uname -s)
ifeq ($(UNAME_S), Linux)
  ECHO =	echo -e
else ifeq ($(UNAME_S), Darwin)
  ECHO =	echo
endif


.PHONY: all debug sanitize me_cry clean fclean mrproper re

all:
	$(MAKE) -C .. $(FLAGS)
	$(MAKE) $(NAME) $(FLAGS)
	./$(NAME)

debug: FLAGS = "CFLAGS = -D DEBUG_MODE -g -ggdb"
debug: all

sanitize: FLAGS = "CFLAGS = -D DEBUG_MODE -g -ggdb -fsanitize=address,undefined \
-ferror-limit=5 -O2"
sanitize: all

me_cry: FLAGS = "CFLAGS = -Wpedantic -Wshadow -Wconversion -Wcast-align \
-Wstrict-prototypes -Wmissing-prototypes -Wunreachable-code -Winit-self \
-Wmissing-declarations -Wfloat-equal -Wbad-function-cast -Wundef \
-Waggregate-return -Wstrict-overflow=5 -Wold-style-definition  \
-Wredundant-decls -Wall -Werror -Wextra -O2" #-Wcast-qual -Wpadded
me_cry: all

-include $(DEPS)

$(NAME): $(OBJS) $(LFT) $(MIN) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(I_DIR) $(OBJS) $(TEST_OBJS) -Wl,--whole-archive $(LIBS) -Wl,--no-whole-archive -o $@

$(O_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(I_DIR) -MMD -c $< -o $@

$(OBJS): | $(O_DIR)

$(O_DIR):
	$(MKDIR) $(O_DIR)

clean:
	$(RM) $(O_DIR)

fclean: clean
	$(RM) $(NAME)

mrproper: fclean
	$(MAKE) -C .. mrproper

re: fclean all
