#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mcanal <mcanal@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/11/29 13:16:03 by mcanal            #+#    #+#              #
#    Updated: 2017/04/13 22:15:08 by mc               ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME =	wolf3d

C_NAME =	main.c game_loop.c
C_MAZE =	maze.c draw_map.c
C_SDL =		events.c update_player.c
C_RAY =		raycaster.c coord_helpers.c skybox.c get_wall_coord.c floorcaster.c

O_DIR = obj
VPATH =	src:src/maze:src/sdl:src/raycaster
SRCS = $(C_NAME) $(C_MAZE) $(C_SDL) $(C_RAY)
OBJS =	$(SRCS:%.c=$(O_DIR)/%.o)


DEPS =		$(OBJS:%.o=%.d)

SDL_DIR = SDL
SDL_INSTALL_DIR = $(PWD)/$(SDL_DIR)/install
SDL_I_DIR = `./$(SDL_DIR)/sdl2-config --cflags`
SDL_LIB =	`./$(SDL_DIR)/sdl2-config --libs`
SDL =	$(SDL_DIR)/build/libSDL2.la

LIBFT_DIR =	libft
LIBFT_I_DIR = -I $(LIBFT_DIR)/inc
LIBFT =		$(LIBFT_DIR)/libft.a
LIBFT_LIB = -L$(LIBFT_DIR) -lft

I_DIR =	$(SDL_I_DIR)	$(LIBFT_I_DIR)	-I inc/
LIBS =	$(SDL_LIB)		$(LIBFT_LIB)	-lm

TEX_DIR =	textures

RM =		rm -rf
MKDIR =		mkdir -p
MAKE =		make
MAKEFLAGS = -j 4
ECHO =		echo -e
CC =		$(shell clang --version >/dev/null 2>&1 && echo clang || echo gcc)
CFLAGS =	-Wall -Wextra -Werror -O2
CCFLAGS += -D TEX_DIR=\"$(shell pwd)/$(TEX_DIR)\"

UNAME_S =   $(shell uname -s)
ifeq ($(UNAME_S), Linux)
  ECHO =	echo -e
else ifeq ($(UNAME_S), Darwin)
  ECHO =	echo
endif


.PHONY: all test debug sanitize me_cry clean fclean mrproper re

all: $(SDL)
	$(MAKE) -C $(LIBFT_DIR) $(FLAGS)
	$(MAKE) $(NAME) $(FLAGS)

test:
	$(MAKE) -C test #TODO: handle flags

debug: FLAGS = "CFLAGS = -D DEBUG_MODE -g -ggdb"
debug: all

sanitize: FLAGS = "CFLAGS = -D DEBUG_MODE -g -ggdb -fsanitize=address,undefined -ferror-limit=5 -O2"
sanitize: all

me_cry: FLAGS = "CFLAGS = -Wpedantic -Wshadow -Wconversion -Wcast-align \
-Wstrict-prototypes -Wmissing-prototypes -Wunreachable-code -Winit-self \
-Wmissing-declarations -Wfloat-equal -Wbad-function-cast -Wundef \
-Waggregate-return -Wstrict-overflow=5 -Wold-style-definition  \
-Wredundant-decls -Wall -Werror -Wextra -O2" #-Wcast-qual -Wpadded
me_cry: all

-include $(DEPS)

$(NAME): $(OBJS) $(LIBFT) $(SDL)
	$(CC) $(CFLAGS) $(CCFLAGS) $(I_DIR) $(OBJS) $(LIBS) -o $@

$(SDL):
	$(MKDIR) $(SDL_INSTALL_DIR)
	cd $(SDL_DIR) && ./configure --prefix=$(SDL_INSTALL_DIR)
	$(MAKE) -C $(SDL_DIR)
	$(MAKE) -C $(SDL_DIR) install

$(O_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(CCFLAGS) $(I_DIR) -MMD -c $< -o $@

$(OBJS): | $(O_DIR)

$(O_DIR):
	$(MKDIR) $(O_DIR)

clean:
	$(RM) $(O_DIR)

fclean: clean
	$(RM) $(NAME)

mrproper: fclean
	$(MAKE) -C $(LIBFT_DIR) fclean
	# $(MAKE) -C $(SDL_DIR) distclean || true

re: fclean all
