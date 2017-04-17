/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floorcaster.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/13 22:27:39 by mc                #+#    #+#             */
/*   Updated: 2017/04/17 22:29:51 by mc               ###   ########.fr       */
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
	/* SDL_Rect src; */
	/* SDL_Rect dst; */

	if (!in_map(context->map, floor_coord->x, floor_coord->y))
		return ;

	/* src.x = (int)(fmod(floor_coord->x, TILE_SIZE)); */
	/* src.y = (int)(fmod(floor_coord->y, TILE_SIZE)); */
	/* src.w = 1; */
	/* src.h = 1; */

	/* dst.x = x; */
	/* dst.y = y; */
	/* dst.w = 1; */
	/* dst.h = 1; */

	/* if (MAP_CHAR(context->map->ptr,							\ */
	/* 			 floor_coord->x, floor_coord->y) == EXIT) */
	/* 	SDL_RenderCopy(context->renderer,					\ */
	/* 				   context->textures[TEX_FLOOR_EXIT],	\ */
	/* 				   &src, &dst); */
	/* else */
	/* 	SDL_RenderCopy(context->renderer,					\ */
	/* 				   context->textures[TEX_FLOOR],		\ */
	/* 				   &src, &dst); */

	/* DEBUG(CLR_GREEN"index: %d \n"CLR_RESET, x + y * PROJ_WIDTH); */
	context->screen_pixels[x + y * PROJ_WIDTH]							\
		= *((int *)context->surfaces[MAP_CHAR(context->map->ptr, \
											  floor_coord->x, \
											  floor_coord->y) == EXIT ? \
									 SUR_FLOOR_EXIT : SUR_FLOOR]->pixels \
			+ ((t_uint)(fmod(floor_coord->x, TILE_SIZE))				\
			   + TILE_SIZE * (t_uint)(fmod(floor_coord->y, TILE_SIZE))));
}

/* double get_floor_coord(t_point *dst, t_context *context, \ */
/* 					   double angle, double vert_angle, t_point *wall_coord) */
/* { */
/* 	double floor_dist; */

/* 	floor_dist = tan(vert_angle) * (double)PLAYER_HEIGHT; //(PROJ_HEIGHT - PLAYER_HEIGHT); */

/* 	//fish-eye? */
/* 	/\* floor_dist *= cos(trig_angle(mod2pi(context->me.angle - angle))); *\/ */

/* 	if (LOOKING_RIGHT(angle)) */
/* 		dst->x = context->me.coord.x + cos(trig_angle(angle)) * floor_dist; */
/* 	else */
/* 		dst->x = context->me.coord.x - cos(trig_angle(angle)) * floor_dist; */

/* 	if (LOOKING_DOWN(angle)) */
/* 		dst->y = context->me.coord.y + sin(trig_angle(angle)) * floor_dist; */
/* 	else */
/* 		dst->y = context->me.coord.y - sin(trig_angle(angle)) * floor_dist; */

/* 	return (floor_dist); */
/* } */


void floorcaster(t_context *context, \
				 double wall_dist, double angle, t_point *wall_coord,
				 int x) //TODO: remove one parameter
{
	t_point floor_coord;
	double floor_dist;
	int y;
	double vert_angle;
	int wall_height;


	if (wall_dist < 0 || ZERO(wall_dist))
		y = (PROJ_HEIGHT / 2) + 1;
	else
	{
		wall_height = (int)(WALL_HEIGHT * PROJ_DIST / wall_dist); //TODO: already calculated in raycaster.c
		y = PROJ_HEIGHT - (PROJ_HEIGHT - wall_height) / 2;
	}

	if (y <= (PROJ_HEIGHT / 2)) //shouldn't happen
		y = (PROJ_HEIGHT / 2) + 1;

	/* vert_angle = M_PI_2 \ */
	/* 	- (FOV / 2) * (double)(y - PROJ_HEIGHT / 2) / (double)(PROJ_HEIGHT / 2); */
	vert_angle = atan(wall_dist / (double)PLAYER_HEIGHT);

#ifdef DEBUG_MODE
	/* DEBUG(CLR_BLUE "y: %d ", y); */
	/* DEBUG(CLR_GREEN "ratio: %f ", \ */
	/* 	  (double)(y - PROJ_HEIGHT / 2) / (double)(PROJ_HEIGHT / 2)); */
	/* DEBUG(CLR_YELLOW "vert_angle: %d ", (int)RADTODEG(vert_angle)); */
	/* DEBUG(CLR_GREEN "c: %d ", (int)RADTODEG(atan(wall_dist / (double)PLAYER_HEIGHT))); */
#endif

	while (y < PROJ_HEIGHT)
	{
		/* if ((floor_dist = get_floor_coord(&floor_coord, context, angle, vert_angle, wall_coord)) > 0) */
		if ((floor_dist = tan(vert_angle) * (double)PLAYER_HEIGHT) > 0) //(PROJ_HEIGHT - PLAYER_HEIGHT);
		{
			floor_coord.x = context->me.coord.x + (wall_coord->x - context->me.coord.x) * floor_dist / wall_dist;
			floor_coord.y = context->me.coord.y + (wall_coord->y - context->me.coord.y) * floor_dist / wall_dist;

			draw_floor_pixel(context, &floor_coord, x, y);
		}


#ifdef DEBUG_MODE
		/* if (!in_map(context->map, floor_coord.x, floor_coord.y)) */
		/* { */
		/* 	DEBUG(CLR_RED "floor not in map: %d/%d ", */
		/* 		  (int)(floor_coord.x / TILE_SIZE), (int)(floor_coord.y / TILE_SIZE)); */
		/* 	DEBUG(CLR_MAGENTA "wall.h: %d ", \ */
		/* 		  (int)((double)WALL_HEIGHT / wall_dist * PROJ_DIST)); */
		/* if (RADTODEG(vert_angle) < 80) */
		/* { */
		/* 	DEBUG(CLR_BLUE "dst: %d/%d ", x, y); */
		/* 	DEBUG(CLR_YELLOW "vert_angle: %f ", vert_angle); */
		/* 	return ; */

		/* } */
		/* 	DEBUG(CLR_GREEN "wall:  %f/%f at %f ", \ */
		/* 		  wall_coord->x, wall_coord->y, wall_dist); */
		/* 	DEBUG(CLR_CYAN "floor: %f/%f at %f (or %f ?)\n", \ */
		/* 		  floor_coord.x, floor_coord.y, floor_dist, \ */
		/* 		  (double)(PROJ_HEIGHT / 2) / (double)(y - (PROJ_HEIGHT / 2)) * PROJ_DIST); */
		/* } */
		/* if (!ZERO(floor_dist - wall_dist)) */
		/* { */
		/* 	/\* DEBUG(CLR_YELLOW "z-ctrl: %d ", \ *\/ */
		/* 	/\* 	  (int)(wall_dist / PROJ_DIST * (y - (PROJ_HEIGHT - (PROJ_HEIGHT / 2))))); *\/ */
		/* 	/\* DEBUG(CLR_YELLOW "z-ctrl: %d ", \ *\/ */
		/* 	/\* 	  (int)(wall_dist / tan(vert_angle))); *\/ */
		/* 	/\* DEBUG(CLR_YELLOW "z-test: %d ", \ *\/ */
		/* 	/\* 	  (PROJ_HEIGHT / 2)); *\/ */
		/* 	DEBUG(CLR_MAGENTA "wall.h: %d ", \ */
		/* 		  (int)((double)WALL_HEIGHT / wall_dist * PROJ_DIST)); */
		/* 	DEBUG(CLR_BLUE "dst: %d/%d ", x, y); */
		/* 	DEBUG(CLR_YELLOW "vert_angle: %f ", RADTODEG(vert_angle)); */
		/* 	DEBUG(CLR_GREEN "wall:  %f/%f at %f ", \ */
		/* 		  wall_coord->x, wall_coord->y, wall_dist); */
		/* 	DEBUG(CLR_CYAN "floor: %f/%f at %f (or %f ?)\n", \ */
		/* 		  floor_coord.x, floor_coord.y, floor_dist, \ */
		/* 		  (double)(PROJ_HEIGHT / 2) / (double)(y - (PROJ_HEIGHT / 2)) * PROJ_DIST); */
		/* } */
		/* return; */
#endif

		y++;
		vert_angle -= (double)FOV / (double)PROJ_HEIGHT; //TODO: define
	}
}
