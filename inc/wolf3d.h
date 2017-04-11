/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 00:31:38 by mc                #+#    #+#             */
/*   Updated: 2017/04/11 16:13:46 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# include "libft.h"
# include "SDL.h"
# include <math.h>

# ifdef DEBUG_MODE
#  include <stdio.h>
#  define DEBUG(str, ...) fprintf(stderr, str, ##__VA_ARGS__)
# else
#  define DEBUG(str, ...) do {} while (0)
# endif


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

enum player_action
{
	A_NONE =   0,
	A_UP =     (1 << 0),
	A_DOWN =   (1 << 1),
	A_LEFT =   (1 << 2),
	A_RIGHT =  (1 << 3),
	A_ROLL =   (1 << 4),
	A_UNROLL = (1 << 5),
};
typedef enum player_action t_action_flag;

enum player_status
{
	S_NONE =   0,
	S_LIVE =   (1 << 0),
	S_MAP =    (1 << 1),
	S_RUN =    (1 << 2)
};
typedef enum player_status t_status_flag;

# define SPEED_PER_FRAME 2
# define ROLL_PER_FRAME (M_PI / 64)
# define RUN_BONUS 2

typedef struct s_point t_point;
struct s_point
{
	double x;
	double y;
};

typedef struct s_player t_player;
struct s_player
{
	t_point         coord;
	double          angle;
	t_action_flag   action;
	t_status_flag   status;
};

typedef struct s_context t_context;
struct s_context
{
    SDL_Window	 *window;
	SDL_Renderer *renderer;
	t_arr        *map;
	t_player     me;
};


# define FPS 30
# define MSPF (1000 / FPS)

//BASE

/*
** game_loop.c
*/
t_bool finit(t_context *context);
t_bool init(t_context *context);
void game_loop(t_context *context, t_uint maze_size);
t_bool kthxbye(t_context *context);


#define MIN_WALL_DIST 1

//MAZE
# define MIN_MAZE_SIZE 3
# define MAX_MAZE_SIZE 200
# define INITIAL_MAZE_SIZE 8

enum map_type
{
	WALL = 'w',
	EMPTY = ' ',
	EXIT = 'E'
};
typedef enum map_type e_map_type;

/*
** maze.c
*/
t_bool generate_maze(t_uint size, t_context *context);



//SDL

#define SDL_BLACK 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE
#define SDL_RED   0xff, 0x00, 0x00, SDL_ALPHA_OPAQUE
#define SDL_GREEN 0x00, 0xff, 0x00, SDL_ALPHA_OPAQUE
#define SDL_BLUE  0x00, 0x00, 0xff, SDL_ALPHA_OPAQUE
#define SDL_WHITE 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE

/*
** events.c
*/
void handle_events(t_context *context);


/*
** update_player.c
*/
void update_player(t_context *context);


//RAYCASTER

/*
** coordinate system:
**  +--> x
**  |           all dimensions in pixels
**  v y
**
**  π/2
** π + O        all angles in rad
**  3π/2
*/


# define TILE_SIZE     64
# define WALL_WIDTH    TILE_SIZE
# define WALL_HEIGHT   (WALL_WIDTH / 2)
# define PLAYER_HEIGHT (WALL_HEIGHT / 2)

/*
** player's field of view
*/
# define FOV (M_PI / 3) //1.0471975512


# define PROJ_WIDTH  960
# define PROJ_HEIGHT 600
# define PROJ_CENTER ({PROJ_WIDTH / 2, PROJ_HEIGHT / 2})

/*
** hardcoded for speed
** # define PROJ_DIST  277 //255?
*/
# define PROJ_DIST ((PROJ_WIDTH / 2) / tan(FOV / 2))
# define ANGLE_PER_RAY (FOV / PROJ_WIDTH)


# define LOOKING_RIGHT(angle) ((angle) < M_PI_2 || (angle) > 3 * M_PI_2)
# define LOOKING_LEFT(angle) (!LOOKING_RIGHT(angle))
# define LOOKING_DOWN(angle) ((angle) > M_PI)
# define LOOKING_UP(angle) (!LOOKING_DOWN(angle))
/* # define LOOKING_VERT(angle) () */
/* # define LOOKING_HORI(angle) () */


/*
** raycaster.c
*/
void raycaster(t_context *context);
double get_wall_coord(t_point *dst, t_context *context, double angle);



/*
** coord_helpers.c
*/
double mod2pi(double angle);
double trig_angle(double angle);
double distance(t_point *a, t_point *b, double angle);
t_bool in_map(t_arr *map, double x, double y);



/*
** draw_map.c (TODO: move)
*/
void draw(t_context *context, t_bool force);

#endif
