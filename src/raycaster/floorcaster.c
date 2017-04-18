/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floorcaster.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/13 22:27:39 by mc                #+#    #+#             */
/*   Updated: 2017/04/18 13:40:00 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycaster.h"

//TODO: define raycaster.c blabla
#define MAP_CHAR(MAP, X, Y) (*(*((char **)(MAP) + (int)((Y) / TILE_SIZE)) + (int)((X) / TILE_SIZE)))

#define DOUBLE_PRECISION (1e-6)
#define ZERO(X) ((X) > -DOUBLE_PRECISION * 2 && (X) < DOUBLE_PRECISION * 2)

#define RADTODEG(angle) ((angle) / M_PI * 180)

static void draw_floor_pixel(t_context *context, t_point *floor_coord, int x, int y)
{
	if (!in_map(context->map, floor_coord->x, floor_coord->y)
		|| x >= PROJ_WIDTH || y >= PROJ_HEIGHT)
		return ;

	context->screen_pixels[x + y * PROJ_WIDTH]					 \
		= *((int *)context->surfaces[MAP_CHAR(context->map->ptr, \
											  floor_coord->x, \
											  floor_coord->y) == EXIT ? \
									 SUR_FLOOR_EXIT : SUR_FLOOR]->pixels \
			+ ((t_uint)(fmod(floor_coord->x, TILE_SIZE))				\
			   + TILE_SIZE * (t_uint)(fmod(floor_coord->y, TILE_SIZE))));
}

#define VERT_FOV (FOV * 1.218)
/* #define VERT_FOV (FOV * PROJ_HEIGHT / PROJ_WIDTH) */

void floorcaster(t_context *context, double wall_dist, t_point *wall_coord, int x)
{
	t_point floor_coord;
	double floor_dist;
	int y;
	double vert_angle;
	int floor_height;


	floor_height = (PROJ_HEIGHT - \
					(int)(WALL_HEIGHT * PROJ_DIST / wall_dist)) //TODO: wall.h already calculated in raycaster.c
		/ 2; //other half is the sky, doh

	vert_angle = M_PI_2 - VERT_FOV / 2;
	y = PROJ_HEIGHT - 1;
	while (y >= PROJ_HEIGHT - floor_height)
	{
		/* if ((floor_dist = get_floor_coord(&floor_coord, context, angle, vert_angle, wall_coord)) > 0) */
		if ((floor_dist = tan(vert_angle) * (double)PLAYER_HEIGHT) > 0) //(PROJ_HEIGHT - PLAYER_HEIGHT);
		{
			floor_coord.x = context->me.coord.x + (wall_coord->x - context->me.coord.x) * floor_dist / wall_dist;
			floor_coord.y = context->me.coord.y + (wall_coord->y - context->me.coord.y) * floor_dist / wall_dist;


#ifdef DEBUG_MODE
			if (!in_map(context->map, floor_coord.x, floor_coord.y))
			{
				DEBUG(CLR_RED "floor not in map: %d/%d ",
					  (int)(floor_coord.x / TILE_SIZE), (int)(floor_coord.y / TILE_SIZE));
				DEBUG(CLR_GREEN "wall:  %f/%f at %f ", \
					  wall_coord->x, wall_coord->y, wall_dist);
				DEBUG(CLR_YELLOW "angle: %f ", \
					  RADTODEG(vert_angle));
				DEBUG(CLR_CYAN "floor: %f/%f at %f, h: %d ", \
					  floor_coord.x, floor_coord.y, floor_dist, floor_height);

				return ;
			}
#endif

			draw_floor_pixel(context, &floor_coord, x, y);
		}


		y--;
		vert_angle += (double)VERT_FOV / (double)PROJ_HEIGHT; //TODO: define
	}

	/* DEBUG(CLR_YELLOW "test:%f ctrl:%f ratio:%f diff:%f",	\ */
	/* 	  RADTODEG(vert_angle), RADTODEG(atan(wall_dist / (double)PLAYER_HEIGHT)), \ */
	/* 	  vert_angle / atan(wall_dist / (double)PLAYER_HEIGHT), \ */
	/* 	  RADTODEG(vert_angle - atan(wall_dist / (double)PLAYER_HEIGHT))); */
}
