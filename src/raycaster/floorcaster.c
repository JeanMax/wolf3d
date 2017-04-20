/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floorcaster.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/13 22:27:39 by mc                #+#    #+#             */
/*   Updated: 2017/04/20 13:11:16 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycaster.h"

//TODO: define raycaster.c blabla
#define MAP_CHAR(MAP, X, Y) (*(*((char **)(MAP) + (int)((Y) / TILE_SIZE)) + (int)((X) / TILE_SIZE)))

#define DOUBLE_PRECISION (1e-6)
#define ZERO(X) ((X) > -DOUBLE_PRECISION * 2 && (X) < DOUBLE_PRECISION * 2)

/* #define RADTODEG(angle) ((angle) / M_PI * 180) */

static void draw_floor_pixel(t_context *context, t_point *floor_coord, int x, int y)
{
	if (!in_map(context->map, floor_coord->x, floor_coord->y)
		|| x >= PROJ_WIDTH || y >= PROJ_HEIGHT)
		return ;

	context->screen_pixels[x + y * PROJ_WIDTH]					 \
		= *((t_uint *)context->surfaces[MAP_CHAR(context->map->ptr, \
											  floor_coord->x, \
											  floor_coord->y) == EXIT ? \
									 SUR_FLOOR_EXIT : SUR_FLOOR]->pixels \
			+ ((t_uint)(fmod(floor_coord->x, TILE_SIZE))				\
			   + TILE_SIZE * (t_uint)(fmod(floor_coord->y, TILE_SIZE))));
}

/* #define VERT_FOV (FOV * 1.218) */
/* #define VERT_FOV (FOV * PROJ_HEIGHT / PROJ_WIDTH) */
#define VERT_FOV PI_6

void floorcaster(t_context *context, int x, t_polar_point *wall)
{
	int y;
	t_polar_point floor;
	int floor_height;


	floor_height = (PROJ_HEIGHT - \
					(int)(WALL_HEIGHT * PROJ_DIST / wall->dist)) //TODO: wall.h already calculated in raycaster.c
		/ 2; //other half is the sky, doh
	/* floor.angle = PI_2 - VERT_FOV / 2; */
	/* y = PROJ_HEIGHT - 1; */

	floor.angle = RAD_TO_ANGLE(atan(wall->dist / (double)PLAYER_HEIGHT));
	y = PROJ_HEIGHT - floor_height;

	/* if (floor_height > 0) */
	/* 	DEBUG(CLR_GREEN"f.angle:%d limit:%d ratio:%f f.h:%d",	\ */
	/* 		  floor.angle,										\ */
	/* 		  VERT_FOV,											\ */
	/* 		  (double)(floor.angle - VERT_FOV) / (double)floor_height, \ */
	/* 		  floor_height */
	/* 	); */

	/* while (y >= PROJ_HEIGHT - floor_height) */
	while (y < PROJ_HEIGHT)
	{
		if ((floor.dist = context->tan_table[floor.angle] * (double)PLAYER_HEIGHT) > 0) //(PROJ_HEIGHT - PLAYER_HEIGHT);
		{
			floor.coord.x = context->me.coord.x + (wall->coord.x - context->me.coord.x) * floor.dist / wall->dist;
			floor.coord.y = context->me.coord.y + (wall->coord.y - context->me.coord.y) * floor.dist / wall->dist;


/* #ifdef DEBUG_MODE */
			/* if (!in_map(context->map, floor.coord.x, floor.coord.y)) */
			/* { */
			/* 	DEBUG(CLR_RED "floor not in map: %d/%d ", */
			/* 		  (int)(floor.coord.x / TILE_SIZE), (int)(floor.coord.y / TILE_SIZE)); */
			/* 	DEBUG(CLR_GREEN "wall:  %f/%f at %f ", \ */
			/* 		  wall->coord.x, wall->coord.y, wall->dist); */
			/* 	DEBUG(CLR_YELLOW "angle: %f ", \ */
			/* 		  RADTODEG(floor.angle)); */
			/* 	DEBUG(CLR_CYAN "floor: %f/%f at %f, h: %d ", \ */
			/* 		  floor.coord.x, floor.coord.y, floor.dist, floor_height); */

			/* 	return ; */
			/* } */
/* #endif */

			draw_floor_pixel(context, &floor.coord, x, y);
		}


		/* y--; */
		/* floor.angle += (double)VERT_FOV / (double)PROJ_HEIGHT; //TODO: define */
		y++;
		floor.angle -= 3;
	}
	/* if (floor_height > 0) */
		/* DEBUG(CLR_YELLOW"angle end:%d",floor.angle); */

	/* DEBUG(CLR_YELLOW "test:%f ctrl:%f ratio:%f diff:%f",	\ */
	/* 	  RADTODEG(floor.angle), RADTODEG(atan(wall->dist / (double)PLAYER_HEIGHT)), \ */
	/* 	  floor.angle / atan(wall->dist / (double)PLAYER_HEIGHT), \ */
	/* 	  RADTODEG(floor.angle - atan(wall->dist / (double)PLAYER_HEIGHT))); */
}
