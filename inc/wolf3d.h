/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcanal <mc.maxcanal@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/12 18:21:18 by mcanal            #+#    #+#             */
/*   Updated: 2017/09/12 18:34:07 by mcanal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# include "libft.h"
# include <SDL.h>
# include <math.h>

# ifdef DEBUG_MODE
# else
#  define DEBUG(str, ...) do {} while (0)
# endif

/*
** used to check equality between doubles
*/

# define DOUBLE_PRECISION (1e-6)
# define ZERO(X) ((X) > -DOUBLE_PRECISION * 2 && (X) < DOUBLE_PRECISION * 2)

/*
** coordinate system:
**	+--> x
**	|			all dimensions in pixels
**	v y
**
**	π/2
** π + O		all angles in 'ANGLE' (this is our maximal precision)
**	3π/2
*/

# define PROJ_WIDTH	960
# define PROJ_HEIGHT 600

/*
** TRIG TABLES
** ((int)((M_PI * 2.) / FOV * PROJ_WIDTH))
** (M_PI * 2. / (double)ANGLE_MAX)
*/

# define ANGLE_MAX 5760
# define ANGLE_UNIT 0.00109083078249645

# define ANGLE_TO_RAD(a) ((double)(a) * ANGLE_UNIT)
# define RAD_TO_ANGLE(a) ((int)((a) / ANGLE_UNIT))

# define PI	2880
# define PI_2 1440
# define PI_3 960
# define PI_4 720
# define PI_6 480

/*
** player's field of view
** (M_PI / 3.)
*/

# define FOV PI_3

# define TILE_SIZE	512
# define WALL_WIDTH	TILE_SIZE
# define WALL_HEIGHT	WALL_WIDTH

# define PLAYER_HEIGHT (WALL_HEIGHT / 2)

/*
** hardcoded for speed
** # define PROJ_DIST ((PROJ_WIDTH / 2) / tan(FOV / 2)) // 831
** (FOV / PROJ_WIDTH)
*/

# define PROJ_DIST	0xff
# define ANGLE_PER_RAY 1

# define LOOKING_RIGHT(angle) ((angle) < PI_2 || (angle) > 3 * PI_2)
# define LOOKING_LEFT(angle) (!LOOKING_RIGHT(angle))
# define LOOKING_DOWN(angle) ((angle) > PI)
# define LOOKING_UP(angle) (!LOOKING_DOWN(angle))
# define LOOKING_VERT(angle) (angle == PI_2 || angle == 3 * PI_2)
# define LOOKING_HORI(angle) (!angle || angle == PI || angle == 2 * PI)

# define MIN_MAZE_SIZE 3
# define MAX_MAZE_SIZE (PROJ_HEIGHT / 2)
# define INITIAL_MAZE_SIZE 8

# define TO_MAP(d) ((int)((d) / (double)TILE_SIZE))
# define MAP_CHAR(MAP, X, Y) (*(*((char **)(MAP) + TO_MAP(Y)) + TO_MAP(X)))

enum						e_map_type
{
	WALL = 'w',
	EMPTY = ' ',
	EXIT = 'E'
};
typedef enum e_map_type		t_map_type;

enum						e_player_action
{
	A_NONE = 0,
	A_UP = (1 << 0),
	A_DOWN = (1 << 1),
	A_LEFT = (1 << 2),
	A_RIGHT = (1 << 3),
	A_ROLL = (1 << 4),
	A_UNROLL = (1 << 5),
};
typedef enum e_player_action	t_action_flag;

enum						e_player_status
{
	S_NONE = 0,
	S_LIVE = (1 << 0),
	S_MAP = (1 << 1),
	S_RUN = (1 << 2)
};
typedef enum e_player_status	t_status_flag;

typedef struct s_point		t_point;
struct						s_point
{
	double	x;
	double	y;
};

typedef struct s_polar_point	t_polar_point;
struct						s_polar_point
{
	t_point		coord;
	int			angle;
	double		dist;
};

typedef struct s_player		t_player;
struct						s_player
{
	t_point			coord;
	int				angle;
	t_action_flag	action;
	t_status_flag	status;
};

enum						e_surface_index
{
	SUR_WALL = 0,
	SUR_SKY = 1,
	SUR_FLOOR = 2,
	SUR_FLOOR_EXIT = 3,
	MAX_SUR = 4
};
typedef enum e_surface_index	t_surface_index;

typedef struct s_context	t_context;
struct						s_context
{
	t_uint			screen_pixels[PROJ_HEIGHT * PROJ_WIDTH];
	SDL_Texture		*screen_texture;
	SDL_Surface		*surfaces[MAX_SUR];
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	t_arr			*map;
	t_player		me;
	double			cos_table[ANGLE_MAX];
	double			sin_table[ANGLE_MAX];
	double			tan_table[ANGLE_MAX];
};

/*
** BASE
** game_loop.c
*/

t_bool						finit(t_context *context);
t_bool						init(t_context *context);
void						game_loop(t_context *context, t_uint maze_size);
t_bool						kthxbye(t_context *context);

/*
** trig_tables.c
*/

void						init_tables(t_context *context);

/*
** MAZE
** maze.c
*/

t_bool						generate_maze(t_uint size, t_context *context);

/*
** draw_map.c
*/

void						draw_map(t_context *context);

/*
** SDL
** events.c
*/

void						handle_events(t_context *context);

/*
** update_player.c
*/

void						update_player(t_context *context);

/*
** draw.c
*/

void						draw(t_context *context, t_bool force);
void						draw_line(t_uint *screen_pixels, t_point *a,
									t_point *b, t_uint color);
void						draw_rect(t_uint *screen_pixels,
									SDL_Rect *rect, t_uint color);

/*
** RAYCASTER
** raycaster.c
*/

void						raycaster(t_context *context);
t_bool						get_wall(t_context *context, t_polar_point *wall);

/*
** coord_helpers.c
*/

int							mod2pi(int angle);
int							trig_angle(int angle);
double						distance(t_point *a, t_point *b, int angle,
									t_context *context);
t_bool						in_map(t_arr *map, double x, double y);

#endif
