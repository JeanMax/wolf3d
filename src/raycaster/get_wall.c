/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_wall.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/13 22:04:51 by mc                #+#    #+#             */
/*   Updated: 2017/04/20 21:08:57 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycaster.h"

static t_bool get_intersection_coord(t_arr *map, t_point *dst, t_point *inc)
{
	while (in_map(map, dst->x, dst->y))
	{
		if (MAP_CHAR(map->ptr, dst->x, dst->y) == WALL)
			return (TRUE);

		//damn, we are on the junction of 4 tiles :o
		if (ZERO(remainder(dst->x, TILE_SIZE)) \
			&& ZERO(remainder(dst->y, TILE_SIZE)))
		{
			if (in_map(map, dst->x + 1, dst->y - 1) \
				&& (MAP_CHAR(map->ptr, dst->x + 1, dst->y - 1) == WALL))
				return (TRUE);
			if (in_map(map, dst->x - 1, dst->y + 1) \
				&& (MAP_CHAR(map->ptr, dst->x - 1, dst->y + 1) == WALL))
				return (TRUE);
			if (in_map(map, dst->x - 1, dst->y - 1)						\
				&& (MAP_CHAR(map->ptr, dst->x - 1, dst->y - 1) == WALL))
				return (TRUE);
			if (in_map(map, dst->x + 1, dst->y + 1)						\
				&& (MAP_CHAR(map->ptr, dst->x + 1, dst->y + 1) == WALL))
				return (TRUE);
		}

		dst->x += inc->x;
		dst->y += inc->y;
	}

	return (FALSE);
}


static t_bool check_intersection_v(t_context *context, t_polar_point *wall)
{
	t_point inc;

	wall->coord.x = (int)(context->me.coord.x / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
	inc.x = TILE_SIZE;
	if (LOOKING_LEFT(wall->angle))
	{
		wall->coord.x -= TILE_SIZE + DOUBLE_PRECISION;
		inc.x = -TILE_SIZE;
	}

	if (LOOKING_HORI(wall->angle))
	{
		wall->coord.y = context->me.coord.y;
		inc.y = 0;
		return (get_intersection_coord(context->map, &wall->coord, &inc));
	}

	inc.y = context->tan_table[trig_angle(wall->angle)];
	if (LOOKING_DOWN(wall->angle))
	{
		wall->coord.y = context->me.coord.y + ABS(context->me.coord.x - wall->coord.x) * inc.y;
		inc.y *= TILE_SIZE;
		return (get_intersection_coord(context->map, &wall->coord, &inc));
	}

	wall->coord.y = context->me.coord.y - ABS(context->me.coord.x - wall->coord.x) * inc.y;
	inc.y *= -TILE_SIZE;
	return (get_intersection_coord(context->map, &wall->coord, &inc));
}

static t_bool check_intersection_h(t_context *context, t_polar_point *wall)
{
	t_point inc;

	wall->coord.y = (int)(context->me.coord.y / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
	inc.y = TILE_SIZE;
	if (LOOKING_UP(wall->angle))
	{
		wall->coord.y -= TILE_SIZE + DOUBLE_PRECISION;
		inc.y = -TILE_SIZE;
	}

	if (LOOKING_VERT(wall->angle))
	{
		wall->coord.x = context->me.coord.x;
		inc.x = 0;
		return (get_intersection_coord(context->map, &wall->coord, &inc));
	}

	inc.x = context->tan_table[trig_angle(wall->angle)];
	if (LOOKING_RIGHT(wall->angle))
	{
		wall->coord.x = context->me.coord.x + ABS(context->me.coord.y - wall->coord.y) / inc.x;
		inc.x = TILE_SIZE / inc.x;
		return (get_intersection_coord(context->map, &wall->coord, &inc));
	}

	wall->coord.x = context->me.coord.x - ABS(context->me.coord.y - wall->coord.y) / inc.x;
	inc.x = -(TILE_SIZE / inc.x);
	return (get_intersection_coord(context->map, &wall->coord, &inc));
}

static t_bool pick_closest_wall(t_polar_point *wall, t_polar_point *wall_h, \
								t_point *me, t_context *context)
{
	if (wall_h->coord.x > 0 && wall->coord.x > 0)
	{
		wall->dist = distance(me, &wall->coord, wall->angle, context);
		wall_h->dist = distance(me, &wall_h->coord, wall->angle, context);
		if (wall_h->dist < wall->dist) //hori
			ft_memcpy(wall, wall_h, sizeof(t_polar_point));
		return (wall->dist > 0);
	}

	if (wall->coord.x > 0) //vert
		return ((wall->dist \
				 = distance(me, &wall->coord, wall->angle, context)) > 0);

	if (wall_h->coord.x > 0) //hori
	{
		ft_memcpy(&wall->coord, &wall_h->coord, sizeof(t_point));
		return ((wall->dist \
				 = distance(me, &wall_h->coord, wall->angle, context)) > 0);
	}

	wall->dist = -1.;
	return (FALSE);
}

/**
** store coordinates and distance of the wall in sight in WALL
** @param: starting from CONTEXT->me.coord at the given WALL.ANGLE
** @return: FALSE if no wall found
*/
t_bool get_wall(t_context *context, t_polar_point *wall)
{
	t_polar_point wall_h;

	wall->coord.x = -1;
	wall_h.coord.x = -1;
	wall_h.angle = wall->angle;

	if (!LOOKING_VERT(wall->angle))
		check_intersection_v(context, wall);
	if (!LOOKING_HORI(wall->angle))
		check_intersection_h(context, &wall_h);

	return (pick_closest_wall(wall, &wall_h, &context->me.coord, context));
}
