/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/26 00:14:11 by mc                #+#    #+#             */
/*   Updated: 2017/04/13 22:09:35 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "raycaster.h"

#define MAP_CHAR(MAP, X, Y) (*(*((char **)(MAP) + (int)((Y) / TILE_SIZE)) + (int)((X) / TILE_SIZE)))

#define DOUBLE_PRECISION (1e-6)
#define ZERO(X) ((X) > -DOUBLE_PRECISION * 2 && (X) < DOUBLE_PRECISION * 2)

static int pick_stripe(t_point *wall_coord)
{
	/* DEBUG(CLR_GREEN"wall -> "CLR_MAGENTA"x:%f "CLR_BLUE"y:%f\n"CLR_RESET, \ */
	/* 		  wall_coord->x, wall_coord->y); */

	/* DEBUG("%sstripe -> %d\n\n"CLR_RESET,				\ */
	/* 	  ZERO(remainder(wall_coord->x, TILE_SIZE)) ? */
	/* 	  CLR_BLUE : CLR_MAGENTA, */

	/* 	  (ZERO(remainder(wall_coord->x, TILE_SIZE)) ? */
	/* 	   (int)(fmod(ABS(wall_coord->y), TILE_SIZE)) : */
	/* 	   (int)(fmod(ABS(wall_coord->x), TILE_SIZE)))); */


	if (ZERO(remainder(wall_coord->x, TILE_SIZE))) //vert
		return ((int)(fmod(ABS(wall_coord->y), TILE_SIZE)));
	return ((int)(fmod(ABS(wall_coord->x), TILE_SIZE)));
}

static void draw_wall(t_context *context, int dst_x, int src_x, \
					  double wall_dist)
{
	SDL_Rect src;
	SDL_Rect dst;

	//TODO: hardcode PROJ_DIST?
	if (wall_dist < 0 || ZERO(wall_dist))
		return ;//TODO: catch these weird stuff if they happen
	dst.h = (int)(WALL_HEIGHT / wall_dist * PROJ_DIST);
	if (dst.h > PROJ_HEIGHT)
	{
		src.h = (int)((double)PROJ_HEIGHT / (double)dst.h * TILE_SIZE);
		src.y = (TILE_SIZE - src.h) / 2; //TODO: z axis
		dst.h = PROJ_HEIGHT;
	}
	else if (dst.h < 1)
		return ; //TODO: catch these weird stuff if they happen
	else
	{
		src.y = 0;
		src.h = TILE_SIZE;
	}
	dst.x = dst_x;
	dst.y = PLAYER_HEIGHT - dst.h / 2;
	//TODO: add a variable on "PROJ_HEIGHT / 2", with keyboard stuffs: z axis
	dst.w = 1;

	src.x = src_x;
	src.w = 1;

	SDL_RenderCopy(context->renderer, context->textures[TEX_WALL],
				   &src, &dst);
	//TODO: catch errors (check all SDL_blahblah call by the way)
}

static double correct_fisheye(double angle, double view_angle, double dist) //TODO: macro?
{
	return (cos(trig_angle(mod2pi(view_angle - angle))) * dist);
}

/**
 ** draw the world in 3d, as you human see it (no?)
 ** @param: CONTEXT used for map, player and renderer infos
 */
void raycaster(t_context *context)
{
	int x;
	double angle;
	double wall_dist;
	t_point wall_coord;

	skybox(context);
	angle = context->me.angle - FOV / 2;
	x = PROJ_WIDTH - 1;
	while (x >= 0)
	{
		if ((wall_dist = get_wall_coord(&wall_coord, context, angle)) > 0)
			draw_wall(context, x, \
					  pick_stripe(&wall_coord),
					  correct_fisheye(angle, context->me.angle, wall_dist));

		angle = mod2pi(angle + ANGLE_PER_RAY);
		x--;
	}
}
