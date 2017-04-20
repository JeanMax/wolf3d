#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mcanal <mcanal@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/11/29 13:16:03 by mcanal            #+#    #+#              #
#    Updated: 2017/04/20 20:42:16 by mc               ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME =		wolfytest

C_NAME =	main.c

C_MAZE =	test_generate_maze.c

C_RAY =	test_get_wall_coord.c

C_SDL =		test_move_player.c

VPATH =		src:src/maze:src/raycaster:src/sdl

O_DIR =		obj
OBJS =		$(C_NAME:%.c=$(O_DIR)/%.o)	\
			$(C_MAZE:%.c=$(O_DIR)/%.o)	\
			$(C_SDL:%.c=$(O_DIR)/%.o)	\
			$(C_RAY:%.c=$(O_DIR)/%.o)

ROOT_DIR =	..

TEST_OBJS =	$(shell find $(ROOT_DIR)/obj -name \*.o 2>/dev/null | grep -v main) #TODO
DEPS =		$(OBJS:%.o=%.d)

LFT_DIR =	$(ROOT_DIR)/libft
LFT_I_DIR = -I$(LFT_DIR)/inc
LFT =		$(LFT_DIR)/libft.a
LFT_LIB =	-L$(LFT_DIR) -lft

MIN_DIR =	$(LFT_DIR)/test/minunit
MIN_I_DIR = -I$(MIN_DIR)
MIN =		$(MIN_DIR)/minunit.h
MIN_LIB =

SDL_DIR =	$(ROOT_DIR)/SDL
SDL_INSTALL_DIR = $(PWD)/$(SDL_DIR)/install
SDL_I_DIR = `./$(SDL_DIR)/sdl2-config --cflags`
SDL_LIB =	`./$(SDL_DIR)/sdl2-config --libs`
SDL =		$(SDL_DIR)/build/libSDL2.la

I_DIR =		$(LFT_I_DIR)	$(MIN_I_DIR)	$(SDL_I_DIR)	-Iinc	-I$(ROOT_DIR)/inc
LIBS =		$(LFT_LIB)		$(MIN_LIB)		$(SDL_LIB)		-lm

RM =		rm -rf
MKDIR =		mkdir -p
MAKE =		make
MAKEFLAGS = -j 4
CC =		$(shell clang --version >/dev/null 2>&1 && echo clang || echo gcc)
CFLAGS =	-Wall -Wextra -Werror -O2

PREV_FLAGS_LOG = .previous-flag
PREV_FLAGS = "$(shell cat "$(O_DIR)/$(PREV_FLAGS_LOG)" 2>/dev/null)"

UNAME_S =   $(shell uname -s)
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


.PHONY: all debug sanitize me_cry clean fclean mrproper re

all: | $(O_DIR)
ifeq (,$(findstring fsanitize, $(PREV_FLAGS)))
	$(ECHO) $(FLAGS) | grep -q fsanitize && $(MAKE) -C $(ROOT_DIR) mrproper && $(MKDIR) $(O_DIR) || true
else
	$(ECHO) $(FLAGS) | grep -qv fsanitize && $(MAKE) -C $(ROOT_DIR) mrproper && $(MKDIR) $(O_DIR) || true
endif
	$(ECHO) $(FLAGS) > $(O_DIR)/$(PREV_FLAGS_LOG)

	$(MAKE) -C $(ROOT_DIR) $(FLAGS)
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

clean:
	$(RM) $(O_DIR)

fclean: clean
	$(RM) $(NAME)

mrproper: fclean
	$(MAKE) -C $(ROOT_DIR) mrproper

re: fclean all


-include $(DEPS)

$(NAME): $(OBJS) $(LFT) $(MIN) $(TEST_OBJS)
	@$(ECHO) "$(BLUE)$(OBJS) $(TEST_OBJS)$(WHITE)->$(RED) $@ $(BASIC)"
	$(CC) $(CFLAGS) $(CCFLAGS) $(I_DIR) $(OBJS) $(TEST_OBJS) -Wl,--whole-archive $(LIBS) -Wl,--no-whole-archive -o $@
	@$(ECHO) "$(WHITE)cflags:$(BASIC) $(CFLAGS)"
	@$(ECHO) "$(WHITE)ccflags:$(BASIC) $(CCFLAGS)"
	@$(ECHO) "$(WHITE)libs:$(BASIC) $(LIBS)"
	@$(ECHO) "$(WHITE)compi:$(BASIC) $(CC)"

$(O_DIR)/%.o: %.c
	@$(ECHO) "$(WHITE)$<\t->$(BLUE) $@ $(BASIC)"
	$(CC) $(CFLAGS) $(CCFLAGS) $(I_DIR) -MMD -c $< -o $@

$(O_DIR):
	$(MKDIR) $(O_DIR)
