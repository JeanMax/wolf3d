/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/26 00:14:11 by mc                #+#    #+#             */
/*   Updated: 2017/03/28 17:57:54 by mc               ###   ########.fr       */
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


static void draw_floor_and_sky(SDL_Renderer *renderer)
{
	SDL_Rect rect;

	//TODO: floor is... black

	ft_bzero(&rect, sizeof(rect));
	rect.w = PROJ_WIDTH;
	rect.h = PROJ_HEIGHT / 2;

	SDL_SetRenderDrawColor(renderer, BLUE);
	SDL_RenderFillRect(renderer, &rect);
}

static void draw_wall(SDL_Renderer *renderer, int x, double wall_dist)
{
	int half_wall_height;

	//TODO: hardcode PROJ_WIDTH?
	//TODO fisheye
	if (wall_dist <= 0)
		return ;//TODO: catch these weird stuff if they happen
	half_wall_height = WALL_HEIGHT / (int)wall_dist * PROJ_WIDTH / 2;
	if (half_wall_height > PROJ_HEIGHT / 2)
		half_wall_height = PROJ_HEIGHT / 2;
	else if (half_wall_height < 1)
		return ; //TODO: catch these weird stuff if they happen

	SDL_SetRenderDrawColor(renderer, GREEN); //TODO: color
	SDL_RenderDrawLine(renderer, x, PROJ_HEIGHT / 2 - half_wall_height, \
								x, PROJ_HEIGHT / 2 + half_wall_height);
}


static double distance(t_point *a, t_point *b) //TODO: use a define instead?
{
	return (sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2)));
	/* return (ABS(me->coord.x - dst->x) / cos(angle)); */ //TODO: use trigo?
}

static t_bool in_map(t_arr *map, t_point *p)
{
	return (p->x >= 0							\
			&& p->y >= 0						\
			&& p->x < map->length * TILE_SIZE	\
			&& p->y < map->length * TILE_SIZE);
}


static t_bool get_intersection_coord(t_arr *map, t_point *dst, t_point *inc)
{
	while (in_map(map, dst))
	{
		if (MAP_CHAR(map->ptr, dst->x, dst->y) == WALL)
			return (TRUE);
		dst->x += inc->x;
		dst->y += inc->y;
	}

	return (FALSE);
}

static t_bool check_intersection_v(t_point *dst, double angle, \
								   t_arr *map, t_player *me)
{
	t_point inc;

	if (angle < M_PI / 2 || angle >= M_PI * 3 / 2) //looking right, x++
	{
		dst->x = floor(me->coord.x / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
		inc.x = TILE_SIZE;
	}
	else
	{
		dst->x = floor(me->coord.x / TILE_SIZE) * TILE_SIZE - 1;
		inc.x = -TILE_SIZE;
	}

	angle = tan(angle);
	if (ZERO(angle))
		return (FALSE);
	dst->y = me->coord.y + (me->coord.x - dst->x) * angle;
	inc.y = TILE_SIZE / angle;

	return ((t_bool)get_intersection_coord(map, dst, &inc));
}

static t_bool check_intersection_h(t_point *dst, double angle, \
								   t_arr *map, t_player *me)
{
	t_point inc;

	if (angle > M_PI) //looking down, y++
	{
		dst->y = floor(me->coord.y / TILE_SIZE) * TILE_SIZE - 1;
		inc.y = TILE_SIZE;
	}
	else
	{
		dst->y = floor(me->coord.y / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
		inc.y = -TILE_SIZE;
	}

	angle = tan(angle);
	if (ZERO(angle))
		return (FALSE);
	dst->x = me->coord.x + (me->coord.y - dst->y) / angle;
	inc.x = TILE_SIZE / angle;

	return (get_intersection_coord(map, dst, &inc));
}

/**
** store coordinates of the wall in sight
** @param: starting from CONTEXT->me.coord at the given ANGLE, stored in *DST
** @return: -1 if no wall found, otherwise the distance from CONTEXT.me to DST
*/
double get_wall_coord(t_point *dst, t_context *context, double angle)
{
	t_point tmp;
	double wall_dist_h;
	double wall_dist_v;

	dst->x = -1;
	tmp.x = -1;
	check_intersection_v(dst, angle, context->map, &context->me);
	check_intersection_h(&tmp, angle, context->map, &context->me);

	if (tmp.x > 0 && dst->x > 0)
	{
		wall_dist_v = distance(&context->me.coord, dst);
		wall_dist_h = distance(&context->me.coord, &tmp);
		if (wall_dist_h < wall_dist_v)
		{
			ft_memcpy(dst, &tmp, sizeof(t_point));
			return (wall_dist_h);
		}
		return (wall_dist_v);
	}

	if (dst->x > 0)
		return (distance(&context->me.coord, dst));

	if (tmp.x > 0)
	{
		ft_memcpy(dst, &tmp, sizeof(t_point));
		return (distance(&context->me.coord, &tmp));
	}

	ft_bzero(dst, sizeof(t_point));
	return (-1); //TODO: throw error?
	//this should never happen if the map is bordered with walls
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

	draw_floor_and_sky(context->renderer);
	angle = context->me.angle - FOV / 2;
	x = 0;
	while (x < PROJ_WIDTH)
	{
		if (angle < 0)
			angle += 2 * M_PI;
		if (angle >= 2 * M_PI)
			angle -= 2 * M_PI;

		if ((wall_dist = get_wall_coord(&wall_coord, context, angle)) > 0)
			draw_wall(context->renderer, x, wall_dist);

		angle += ANGLE_PER_RAY;
		x++;
	}

	/* printf("me: x:%f, y:%f, angle:%f \n", context->me.coord.x / TILE_SIZE, context->me.coord.y / TILE_SIZE, context->me.angle);	/\* DEBUG *\/ */
}
