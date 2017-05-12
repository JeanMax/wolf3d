/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:09:15 by mc                #+#    #+#             */
/*   Updated: 2017/04/20 13:46:36 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SDL_H
# define SDL_H

# include <time.h> //bonus
# include <stdlib.h>

# include "wolf3d.h"

# define KEYA_UP     SDLK_UP
# define KEYA_DOWN   SDLK_DOWN
# define KEYA_LEFT   SDLK_LEFT
# define KEYA_RIGHT  SDLK_RIGHT
# define KEYA_UNROLL SDLK_PAGEDOWN
# define KEYA_ROLL   SDLK_PAGEUP
# define KEYA_RUN    SDLK_RSHIFT
# define KEYA_MAP    SDLK_BACKSPACE

# ifdef QWERTY
#  define KEYB_UP     SDLK_w
#  define KEYB_DOWN   SDLK_s
#  define KEYB_LEFT   SDLK_a
#  define KEYB_RIGHT  SDLK_d
#  define KEYB_UNROLL SDLK_e
#  define KEYB_ROLL   SDLK_q
# else
#  define KEYB_UP     SDLK_z
#  define KEYB_DOWN   SDLK_s
#  define KEYB_LEFT   SDLK_q
#  define KEYB_RIGHT  SDLK_d
#  define KEYB_UNROLL SDLK_e
#  define KEYB_ROLL   SDLK_a
# endif
# define KEYB_RUN    SDLK_LSHIFT
# define KEYB_MAP    SDLK_TAB

# define SPEED_PER_FRAME (TILE_SIZE / 16)
# define ROLL_PER_FRAME (PI / 40)
# define RUN_BONUS 2

# define MIN_WALL_DIST 1

# define FPS 24
# define MSPF (1000 / FPS) //MilliSecondPerFrame

/*
** update_player.c
*/
void move_player(t_context *context, int angle, double distance);

/*
** event_handlers-move.c
*/
t_bool handler_move_up(t_context *context, int key, t_uint type);
t_bool handler_move_down(t_context *context, int key, t_uint type);
t_bool handler_move_left(t_context *context, int key, t_uint type);
t_bool handler_move_right(t_context *context, int key, t_uint type);

/*
** event_handlers-roll.c
*/
t_bool handler_roll(t_context *context, int key, t_uint type);
t_bool handler_unroll(t_context *context, int key, t_uint type);

/*
**  event_handlers-status.c
*/
t_bool handler_run(t_context *context, int key, t_uint type);
t_bool handler_map(t_context *context, int key, t_uint type);

#endif
