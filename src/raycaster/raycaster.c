/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/26 00:14:11 by mc                #+#    #+#             */
/*   Updated: 2017/03/27 20:39:01 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "raycaster.h"

#include <stdio.h>				/* DEBUG */

#define MAP_CHAR(MAP, X, Y) (*(*((char **)(MAP) + (int)(Y / TILE_SIZE)) + (int)(X / TILE_SIZE)))
#define ABS(X) ((X) < 0 ? (-X) : (X))
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define ZERO(X) ((X) > -1e-3 && (X) < 1e-3)

static t_bool in_map(t_arr *map, t_point *p)
{
	return (p->x >= 0							\
			&& p->y >= 0						\
			&& p->x < map->length * TILE_SIZE	\
			&& p->y < map->length * TILE_SIZE);
}

static t_bool get_intersection_coord(t_arr *map, t_point *grid_inter, \
									 t_point *inc)
{
	//part 8 ; TODO fisheye
	while (in_map(map, grid_inter))
	{
		if (MAP_CHAR(map->ptr, grid_inter->x, grid_inter->y) == WALL)
			return (TRUE);
		grid_inter->x += inc->x;
		grid_inter->y += inc->y;
	}

	return (FALSE);
}

static double check_intersection_v(t_arr *map, t_player *me, \
										double cast_angle)
{
	//part 7
	t_point grid_inter;
	t_point inc;

	/* if (!cast_angle || cast_angle == M_PI) */
		/* return (-1);  //looking straight to the left/right */

	if (cast_angle < M_PI / 2 || cast_angle >= M_PI * 3 / 2) //looking right, x++
	{
		grid_inter.x = floor(me->coord.x / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
		inc.x = TILE_SIZE;
	}
	else
	{
		grid_inter.x = floor(me->coord.x / TILE_SIZE) * TILE_SIZE - 1;
		inc.x = -TILE_SIZE;
	}

	cast_angle = tan(cast_angle);
	if (ZERO(cast_angle))
		return (-1);
	grid_inter.y = me->coord.y + (me->coord.x - grid_inter.x) * cast_angle;
	inc.y = TILE_SIZE / cast_angle;

	if (get_intersection_coord(map, &grid_inter, &inc))
		return (sqrt(pow(me->coord.x - grid_inter.x, 2) \
					 + pow(me->coord.y - grid_inter.y, 2)));
		/* return (ABS(me->coord.x - grid_inter.x) / cos(cast_angle)); */

	return (-1);
}

static double check_intersection_h(t_arr *map, t_player *me, \
										  double cast_angle)
{
	//part 7
	t_point grid_inter;
	t_point inc;

	/* if (cast_angle == M_PI / 2 || cast_angle == M_PI * 3 / 2) */
		/* return (-1); //looking straight to the top/bottom */

	if (cast_angle > M_PI) //looking down, y++
	{
		grid_inter.y = floor(me->coord.y / TILE_SIZE) * TILE_SIZE - 1;
		inc.y = TILE_SIZE;
	}
	else
	{
		grid_inter.y = floor(me->coord.y / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
		inc.y = -TILE_SIZE;
	}

	cast_angle = tan(cast_angle);
	if (ZERO(cast_angle))
		return (-1);
	grid_inter.x = me->coord.x + (me->coord.y - grid_inter.y) / cast_angle;
	inc.x = TILE_SIZE / cast_angle;

	if (get_intersection_coord(map, &grid_inter, &inc))
		return (sqrt(pow(me->coord.x - grid_inter.x, 2) \
					 + pow(me->coord.y - grid_inter.y, 2)));
		/* return (ABS(me->coord.x - grid_inter.x) / cos(cast_angle)); */

	return (-1);
}

static void draw_floor_and_sky(SDL_Renderer *renderer)
{
	SDL_Rect rect;

	//floor is... black

	ft_bzero(&rect, sizeof(rect));
	rect.w = PROJ_WIDTH;
	rect.h = PROJ_HEIGHT / 2;

	SDL_SetRenderDrawColor(renderer, BLUE);
	SDL_RenderFillRect(renderer, &rect);
}

void raycaster(t_context *context)
{
	//part 6
	int x;
	double cast_angle;
	double wall_dist_h;
	double wall_dist_v;

	draw_floor_and_sky(context->renderer);
	/* printf("me: x:%f, y:%f, angle:%f \n", context->me.coord.x / TILE_SIZE, context->me.coord.y / TILE_SIZE, context->me.angle);	/\* DEBUG *\/ */
	cast_angle = context->me.angle - FOV / 2;
	x = 0;
	while (x < PROJ_WIDTH)
	{
		if (cast_angle < 0)
			cast_angle += 2 * M_PI;
		if (cast_angle >= 2 * M_PI)
			cast_angle -= 2 * M_PI;

		wall_dist_h = check_intersection_h(context->map, &context->me, cast_angle);
		wall_dist_v = check_intersection_v(context->map, &context->me, cast_angle);
		if (wall_dist_h > 0 || wall_dist_v > 0)
		{
			if (wall_dist_h > 0 && wall_dist_v > 0)
				wall_dist_h = MIN(wall_dist_h, wall_dist_v);
			else if (wall_dist_v > 0)
				wall_dist_h = wall_dist_v;

			//TODO: hardcode PROJ_WIDTH?
			wall_dist_h = WALL_HEIGHT / wall_dist_h * PROJ_WIDTH;
			if (wall_dist_h > PROJ_HEIGHT)
				wall_dist_h = PROJ_HEIGHT;
			else if (wall_dist_h < 0)
				wall_dist_h = 0;

			SDL_SetRenderDrawColor(context->renderer, GREEN); //TODO: color
			SDL_RenderDrawLine(context->renderer,						\
							   x, PROJ_HEIGHT / 2 - (int)wall_dist_h / 2, \
							   x, PROJ_HEIGHT / 2 + (int)wall_dist_h / 2);
		}

		cast_angle += ANGLE_PER_RAY;
		x++;
	}
}
