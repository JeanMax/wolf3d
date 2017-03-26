/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 00:31:38 by mc                #+#    #+#             */
/*   Updated: 2017/03/26 01:35:19 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# include "libft.h"
# include "SDL.h"
#include <math.h>



typedef struct s_point t_point;
struct s_point
{
	double x;
	double y;
};

typedef struct s_player t_player;
struct s_player
{
	t_point coord;
	double  angle;
};

typedef struct s_context t_context;
struct s_context
{
    SDL_Window	 *window;
	SDL_Renderer *renderer;
	t_arr        *map;
	t_player     me;
};


# define FPS 50
# define MSPF (1000 / FPS)

/*
** main.c
*/
t_bool kthxbye(t_context *context);


//MAZE
#define INITIAL_MAZE_SIZE 16

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

#define BLACK 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE
#define RED   0xff, 0x00, 0x00, SDL_ALPHA_OPAQUE
#define GREEN 0x00, 0xff, 0x00, SDL_ALPHA_OPAQUE
#define BLUE  0x00, 0x00, 0xff, SDL_ALPHA_OPAQUE
#define WHITE 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE

/*
** events.c
*/
void handle_events(t_context *context);



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
# define WALL_HEIGHT   WALL_WIDTH
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

/*
** raycaster.c
*/
void raycaster(t_context *context);

#endif
