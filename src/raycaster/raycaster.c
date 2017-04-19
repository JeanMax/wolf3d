/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/26 00:14:11 by mc                #+#    #+#             */
/*   Updated: 2017/04/19 21:11:21 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycaster.h"

//TODO:
#define MAP_CHAR(MAP, X, Y) (*(*((char **)(MAP) + (int)((Y) / TILE_SIZE)) + (int)((X) / TILE_SIZE)))

#define DOUBLE_PRECISION (1e-6)
#define ZERO(X) ((X) > -DOUBLE_PRECISION * 2 && (X) < DOUBLE_PRECISION * 2)

#define CLR_WALL_NORTH 0xff
#define CLR_WALL_EAST  0xbb
#define CLR_WALL_SOUTH 0x99
#define CLR_WALL_WEST  0xdd

static int pick_stripe(t_point *wall_coord)
{
	if (ZERO(remainder(wall_coord->x, TILE_SIZE))) //vert
		return ((int)(fmod(wall_coord->y, TILE_SIZE)));
	return ((int)(fmod(wall_coord->x, TILE_SIZE)));
}

static t_uint get_wall_color(t_polar_point *wall)
{
	if (ZERO(remainder(wall->coord.x, TILE_SIZE))) //vert
	{
		if (LOOKING_RIGHT(wall->angle))
			return (CLR_WALL_WEST);
		return (CLR_WALL_EAST);
	}

	if (LOOKING_DOWN(wall->angle))
		return (CLR_WALL_NORTH);
	return (CLR_WALL_SOUTH);
}

static void copy_wall_pixels(t_context *context, SDL_Rect *dst, SDL_Rect *src, t_uint color)
{
	double src_inc;
	double src_y;
	t_uchar xbgr[4];

	src_y = (double)src->y;
	src_inc = (double)src->h / (double)dst->h;
	while (dst->h)
	{
		/* *(int *)xbgr = *(int *)context->surfaces[SUR_WALL]->pixels \ */
				  /* + src->x + TILE_SIZE * (int)src_y; */
		ft_memcpy(&xbgr, (t_uint *)context->surfaces[SUR_WALL]->pixels \
				  + src->x + TILE_SIZE * (int)src_y, sizeof(int)); //TODO: opti
		xbgr[1] = (t_uchar)((double)xbgr[1] * ((double)color / 255.));
		xbgr[2] = (t_uchar)((double)xbgr[2] * ((double)color / 255.));
		xbgr[3] = (t_uchar)((double)xbgr[3] * ((double)color / 255.));

		context->screen_pixels[dst->x + dst->y * PROJ_WIDTH] = *(t_uint *)xbgr;
		/* ft_memcpy(&context->screen_pixels[dst->x + dst->y * PROJ_WIDTH], &xbgr, \ */
				  /* sizeof(int)); */


		/* context->screen_pixels[dst->x + dst->y * PROJ_WIDTH] */
		/* 	= *((int *)context->surfaces[SUR_WALL]->pixels	\ */
		/* 		+ src->x + TILE_SIZE * (int)src_y) | color; */

		src_y += src_inc;
		dst->y++;
		dst->h--;
	}
}

static void draw_wall(t_context *context, int dst_x, t_polar_point *wall)
{
	SDL_Rect src;
	SDL_Rect dst;

	//TODO: hardcode PROJ_DIST?
	dst.h = (int)(WALL_HEIGHT * PROJ_DIST / wall->dist);
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
	dst.y = (PROJ_HEIGHT / 2) - dst.h / 2;
	//TODO: add a variable on "PROJ_HEIGHT / 2", with keyboard stuffs: z axis
	dst.w = 1;

	src.x = pick_stripe(&wall->coord);
	src.w = 1;

	copy_wall_pixels(context, &dst, &src, get_wall_color(wall));
}

/**
 ** draw the world in 3d, as you human see it (no?)
 ** @param: CONTEXT used for map, player and renderer infos
 */
void raycaster(t_context *context)
{
	int x;
	t_polar_point wall;

	skybox(context);

	wall.angle = mod2pi(context->me.angle - FOV / 2);
	x = PROJ_WIDTH - 1;
	while (x >= 0)
	{
		if (get_wall(context, &wall))
		{
			//correct fisheye
			wall.dist *= cos(trig_angle(mod2pi(context->me.angle \
											   - wall.angle)));
			draw_wall(context, x, &wall);
			floorcaster(context, x, &wall);
		}

		wall.angle = mod2pi(wall.angle + ANGLE_PER_RAY);
		x--;
	}
}
