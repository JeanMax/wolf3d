/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/26 00:14:11 by mc                #+#    #+#             */
/*   Updated: 2017/03/26 01:03:09 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "raycaster.h"

#include <stdio.h>				/* DEBUG */

#define MAP_CHAR(MAP, X, Y) (*(*((char **)(MAP) + (int)(Y / TILE_SIZE)) + (int)(X / TILE_SIZE)))
#define ABS(X) ((X) < 0 ? (-X) : (X))

static t_bool in_map(t_arr *map, t_point *p)
{
	return (p->x >= 0							\
			&& p->y >= 0						\
			&& p->x < map->length * TILE_SIZE	\
			&& p->y < map->length * TILE_SIZE);
}

static t_bool get_intersection_coord(t_arr *map, t_point *grid_inter, t_point *inc)
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

static double check_vertical_intersection(t_arr *map, t_player *me, \
										double cast_angle)
{
	//part 7
	t_point grid_inter;
	t_point inc;

	if (!cast_angle || cast_angle == M_PI)
		return (-1);  //looking straight to the left/right

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

	grid_inter.y = me->coord.y + (me->coord.x - grid_inter.x) * tan(cast_angle);
	inc.y = TILE_SIZE / tan(cast_angle);

	if (get_intersection_coord(map, &grid_inter, &inc))
		return (sqrt(pow(me->coord.x - grid_inter.x, 2) \
					 + pow(me->coord.y - grid_inter.y, 2)));
		/* return (ABS(me->coord.x - grid_inter.x) / cos(cast_angle)); */

	return (-1);
}

static double check_horizontal_intersection(t_arr *map, t_player *me, \
										  double cast_angle)
{
	//part 7
	t_point grid_inter;
	t_point inc;

	if (cast_angle == M_PI / 2 || cast_angle == M_PI * 3 / 2)
		return (-1); //looking straight to the top/bottom

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

	grid_inter.x = me->coord.x + (me->coord.y - grid_inter.y) / tan(cast_angle);
	inc.x = TILE_SIZE / tan(cast_angle);

	if (get_intersection_coord(map, &grid_inter, &inc))
		return (sqrt(pow(me->coord.x - grid_inter.x, 2) \
					 + pow(me->coord.y - grid_inter.y, 2)));
		/* return (ABS(me->coord.x - grid_inter.x) / cos(cast_angle)); */

	return (-1);
}

static void draw_floor_and_sky(SDL_Renderer *renderer)
{
	SDL_Rect rect;

	SDL_SetRenderDrawColor(renderer, BLACK);
	SDL_RenderClear(renderer);

	ft_bzero(&rect, sizeof(rect));
	rect.w = PROJ_WIDTH;
	rect.h = PROJ_HEIGHT / 2;

	SDL_SetRenderDrawColor(renderer, BLUE);
	SDL_RenderFillRect(renderer, &rect);
}

void raycaster(t_context *context)
{
	//part 6
	t_uint x;
	double cast_angle;
	double wall_dist;

	draw_floor_and_sky(context->renderer);
	printf("me angle %f \n", context->me.angle);	/* DEBUG */
	cast_angle = context->me.angle - FOV / 2;
	x = 0;
	while (x < PROJ_WIDTH)
	{
		if (cast_angle < 0)
			cast_angle += 2 * M_PI;
		if (cast_angle >= 2 * M_PI)
			cast_angle -= 2 * M_PI;

		if ((wall_dist = check_horizontal_intersection(context->map, &context->me, cast_angle) > 0))
		{
			//TODO: hardcode PROJ_WIDTH?
			SDL_SetRenderDrawColor(context->renderer, GREEN);
			wall_dist = WALL_HEIGHT / wall_dist * PROJ_WIDTH / 2;
			SDL_RenderDrawLine(context->renderer,						\
							   x, PROJ_HEIGHT / 2 - wall_dist,	\
							   x, PROJ_HEIGHT / 2 + wall_dist); //TODO: color
		}
		if ((wall_dist = check_vertical_intersection(context->map, &context->me, cast_angle)) > 0)
		{
			SDL_SetRenderDrawColor(context->renderer, RED);
			//TODO: hardcode PROJ_WIDTH?
			wall_dist = WALL_HEIGHT / wall_dist * PROJ_WIDTH / 2;
			SDL_RenderDrawLine(context->renderer,						\
							   x, PROJ_HEIGHT / 2 - wall_dist,	\
							   x, PROJ_HEIGHT / 2 + wall_dist); //TODO: color
		}

		/* else if (wall_dist != -1) */
		/* { */
		/* 	printf("d:%f, dh:%f, dv:%f \n", wall_dist, check_horizontal_intersection(map, me,cast_angle), check_vertical_intersection(map, me, cast_angle));	 /\* DEBUG *\/ */
		/* 	printf("cast_angle:%f \n", cast_angle);	/\* DEBUG *\/ */
		/* } */

		cast_angle += ANGLE_PER_RAY;
		x++;
	}
}
