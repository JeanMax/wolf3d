/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 00:31:38 by mc                #+#    #+#             */
/*   Updated: 2017/03/25 06:04:32 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# include "libft.h"
# include "SDL.h"


/*
** coordinate system:
**  +--> x
**  |           all dimensions in pixels
**  v y
**
**     90°
** 180° + O°    all angles in degrees
**     270°
*/


# define WALL_HEIGHT   64
# define WALL_WIDTH    WALL_HEIGHT
# define PLAYER_HEIGHT (WALL_HEIGHT / 2)

/*
** player's field of view
*/
# define FOV 60


# define PROJ_WIDTH  960
# define PROJ_HEIGHT 600
# define PROJ_CENTER ({PROJ_WIDTH / 2, PROJ_HEIGHT / 2})

/*
** hardcoded for speed
** # define PROJ_DIST  277
*/
# define PROJ_DIST ((PROJ_WIDTH / 2) / tan(FOV / 2))
# define ANGLE_PER_RAY (FOV / PROJ_WIDTH)

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

# define ANGLE_MOD(a) ((a) % 360)


//MAZE
#define INITIAL_MAZE_SIZE 16

typedef enum map_type e_map_type;
enum map_type
{
	WALL = 'w',
	EMPTY = ' ',
	EXIT = 'E'
};

/*
** maze.c
*/
t_arr *generate_maze(t_uint size, t_player *player, SDL_Renderer *renderer);



//SDL

/*
** events.c
*/
void handle_events(SDL_Renderer *renderer);


#endif
