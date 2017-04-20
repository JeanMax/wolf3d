#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mcanal <mcanal@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/11/29 13:16:03 by mcanal            #+#    #+#              #
#    Updated: 2017/04/20 21:06:22 by mc               ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME =		wolf3d

C_NAME =	main.c			game_loop.c		trig_tables.c

C_MAZE =	maze.c			draw_map.c		maze_helpers.c

C_SDL =		events.c		draw.c			update_player.c			\
			event_handlers-move.c			event_handlers-roll.c	\
			event_handlers-status.c

C_RAY =		raycaster.c		skybox.c		floorcaster.c			\
			get_wall.c		coord_helpers.c

VPATH =		src:src/maze:src/sdl:src/raycaster
O_DIR =		obj
OBJS =		$(C_NAME:%.c=$(O_DIR)/%.o)	\
			$(C_MAZE:%.c=$(O_DIR)/%.o)	\
			$(C_SDL:%.c=$(O_DIR)/%.o)	\
			$(C_RAY:%.c=$(O_DIR)/%.o)

DEPS =		$(OBJS:%.o=%.d)

TEX_DIR =	textures
TEST_DIR =	test

SDL_DIR =	SDL
SDL_INSTALL_DIR = $(PWD)/$(SDL_DIR)/install
SDL_I_DIR =	`./$(SDL_DIR)/sdl2-config --cflags`
SDL_LIB =	`./$(SDL_DIR)/sdl2-config --libs`
SDL =		$(SDL_DIR)/build/libSDL2.la

LFT_DIR =	libft
LFT_I_DIR = -I $(LFT_DIR)/inc
LFT =		$(LFT_DIR)/libft.a
LFT_LIB =	-L$(LFT_DIR) -lft

I_DIR =		$(SDL_I_DIR)	$(LFT_I_DIR)	-I inc/
LIBS =		$(SDL_LIB)		$(LFT_LIB)		-lm

RM =		rm -rf
MKDIR =		mkdir -p
MAKE =		make
MAKEFLAGS = -j 4
CCFLAGS +=	-D TEX_DIR=\"$(shell pwd)/$(TEX_DIR)\"
CFLAGS =	-Wall -Wextra -Werror -O2
CC =		$(shell clang --version >/dev/null 2>&1 && echo clang || echo gcc)

PREV_FLAGS_LOG = .previous-flag
PREV_FLAGS = "$(shell cat "$(O_DIR)/$(PREV_FLAGS_LOG)" 2>/dev/null)"

UNAME_S =	$(shell uname -s)
ifeq ($(UNAME_S), Linux)
  ECHO =	echo -e
else ifeq ($(UNAME_S), Darwin)
  ECHO =	echo
endif

WHITE =		\033[37;01m
RED =		\033[31;01m
GREEN =		\033[32;01m
YELLOW =	\033[33;01m
BLUE =		\033[34;01m
BASIC =		\033[0m

ifndef VERBOSE
.SILENT:
endif


.PHONY: all sdl test debug sanitize me_cry clean fclean mrproper re

all: | $(O_DIR)
ifeq (,$(findstring fsanitize, $(PREV_FLAGS)))
	$(ECHO) $(FLAGS) | grep -q fsanitize && $(MAKE) mrproper && $(MKDIR) $(O_DIR) || true
else
	$(ECHO) $(FLAGS) | grep -qv fsanitize && $(MAKE) mrproper && $(MKDIR) $(O_DIR) || true
endif
	$(ECHO) $(FLAGS) > $(O_DIR)/$(PREV_FLAGS_LOG)

	$(MAKE) sdl
	$(MAKE) -C $(LFT_DIR) $(FLAGS)
	$(MAKE) $(NAME) $(FLAGS)

test: all
	$(MAKE) -C $(TEST_DIR) #TODO: handle flags

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

sdl: $(SDL)

clean:
	$(RM) $(O_DIR)

fclean: clean
	$(RM) $(NAME)

sdlclean:
	$(MAKE) -C $(SDL_DIR) distclean || true

mrproper: fclean
	$(MAKE) -C $(LFT_DIR) fclean
	$(MAKE) -C $(TEST_DIR) fclean || true

re: fclean all


-include $(DEPS)

$(NAME): $(OBJS) $(LFT) $(SDL)
	@$(ECHO) "$(BLUE)$(OBJS) $(WHITE)->$(RED) $@ $(BASIC)"
	$(CC) $(CFLAGS) $(CCFLAGS) $(I_DIR) $(OBJS) $(LIBS) -o $@
	@$(ECHO) "$(WHITE)cflags:$(BASIC) $(CFLAGS)"
	@$(ECHO) "$(WHITE)ccflags:$(BASIC) $(CCFLAGS)"
	@$(ECHO) "$(WHITE)libs:$(BASIC) $(LIBS)"
	@$(ECHO) "$(WHITE)compi:$(BASIC) $(CC)"

$(SDL):
	$(MKDIR) $(SDL_INSTALL_DIR)
	cd $(SDL_DIR) && ./configure --prefix=$(SDL_INSTALL_DIR)
	$(MAKE) -C $(SDL_DIR)
	$(MAKE) -C $(SDL_DIR) install

$(O_DIR)/%.o: %.c
	@$(ECHO) "$(WHITE)$<\t->$(BLUE) $@ $(BASIC)"
	$(CC) $(CFLAGS) $(CCFLAGS) $(I_DIR) -MMD -c $< -o $@

$(O_DIR):
	$(MKDIR) $(O_DIR)
