/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_wall_coord.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/13 22:04:51 by mc                #+#    #+#             */
/*   Updated: 2017/04/19 21:02:57 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycaster.h"

//TODO: centralise define from raycaster.c

#define MAP_CHAR(MAP, X, Y) (*(*((char **)(MAP) + (int)((Y) / TILE_SIZE)) + (int)((X) / TILE_SIZE)))

#define DOUBLE_PRECISION (1e-6)
#define ZERO(X) ((X) > -DOUBLE_PRECISION * 2 && (X) < DOUBLE_PRECISION * 2)

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


static t_bool check_intersection_v(t_polar_point *wall, \
								   t_arr *map, t_player *me)
{
	t_point inc;

	wall->coord.x = (int)(me->coord.x / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
	inc.x = TILE_SIZE;
	if (LOOKING_LEFT(wall->angle))
	{
		wall->coord.x -= TILE_SIZE + DOUBLE_PRECISION;
		inc.x = -TILE_SIZE;
	}

	//looking straight to the left/right
	if (ZERO(mod2pi(wall->angle)) || ZERO(mod2pi(wall->angle - M_PI)) || ZERO(mod2pi(wall->angle - 2 * M_PI)))
	{
		wall->coord.y = me->coord.y;
		inc.y = 0;
		return (get_intersection_coord(map, &wall->coord, &inc));
	}

	inc.y = tan(trig_angle(wall->angle));
	if (LOOKING_DOWN(wall->angle))
	{
		wall->coord.y = me->coord.y + ABS(me->coord.x - wall->coord.x) * inc.y;
		inc.y *= TILE_SIZE;
		return (get_intersection_coord(map, &wall->coord, &inc));
	}

	wall->coord.y = me->coord.y - ABS(me->coord.x - wall->coord.x) * inc.y;
	inc.y *= -TILE_SIZE;
	return (get_intersection_coord(map, &wall->coord, &inc));
}

static t_bool check_intersection_h(t_polar_point *wall, \
								   t_arr *map, t_player *me)
{
	t_point inc;

	wall->coord.y = (int)(me->coord.y / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
	inc.y = TILE_SIZE;
	if (LOOKING_UP(wall->angle))
	{
		wall->coord.y -= TILE_SIZE + DOUBLE_PRECISION;
		inc.y = -TILE_SIZE;
	}

	//looking straight to the top/bottom
	if (ZERO(mod2pi(wall->angle - M_PI_2)) || ZERO(mod2pi(wall->angle - 3 * M_PI_2)))
	{
		wall->coord.x = me->coord.x;
		inc.x = 0;
		return (get_intersection_coord(map, &wall->coord, &inc));
	}

	inc.x = tan(trig_angle(wall->angle));
	if (LOOKING_RIGHT(wall->angle))
	{
		wall->coord.x = me->coord.x + ABS(me->coord.y - wall->coord.y) / inc.x;
		inc.x = TILE_SIZE / inc.x;
		return (get_intersection_coord(map, &wall->coord, &inc));
	}

	wall->coord.x = me->coord.x - ABS(me->coord.y - wall->coord.y) / inc.x;
	inc.x = -(TILE_SIZE / inc.x);
	return (get_intersection_coord(map, &wall->coord, &inc));
}

static t_bool pick_closest_wall(t_polar_point *wall, t_polar_point *wall_h, \
								t_point *me)
{
	if (wall_h->coord.x > 0 && wall->coord.x > 0)
	{
		wall->dist = distance(me, &wall->coord, wall->angle);
		wall_h->dist = distance(me, &wall_h->coord, wall->angle);
		if (wall_h->dist < wall->dist) //hori
			ft_memcpy(wall, wall_h, sizeof(t_polar_point));
		return (wall->dist > 0);
	}

	if (wall->coord.x > 0) //vert
		return ((wall->dist \
			= distance(me, &wall->coord, wall->angle)) > 0);

	if (wall_h->coord.x > 0) //hori
	{
		ft_memcpy(&wall->coord, &wall_h->coord, sizeof(t_point));
		return ((wall->dist \
			= distance(me, &wall_h->coord, wall->angle)) > 0);
	}

	wall->dist = -1.;
	return (FALSE); //TODO: throw error?
	//this should never happen if the map is bordered with walls
}

/**
 ** store coordinates of the wall in sight
 ** @param: starting from CONTEXT->me.coord at the given ANGLE, stored in *DST
 ** @return: -1 if no wall found, otherwise the distance from CONTEXT.me to DST
 */
t_bool get_wall(t_context *context, t_polar_point *wall)
{
	t_polar_point wall_h;

	wall->coord.x = -1;
	wall_h.coord.x = -1;
	wall_h.angle = wall->angle;

	//looking straight to the top/bottom TODO: define
	if (!(ZERO(mod2pi(wall->angle - M_PI_2)) \
		  || ZERO(mod2pi(wall->angle - 3 * M_PI_2))))
		check_intersection_v(wall, context->map, &context->me);
	//looking straight to the left/right TODO: define
	if (!(ZERO(mod2pi(wall->angle)) \
		  || ZERO(mod2pi(wall->angle - M_PI)) \
		  || ZERO(mod2pi(wall->angle - 2 * M_PI))))
		check_intersection_h(&wall_h, context->map, &context->me);

	return (pick_closest_wall(wall, &wall_h, &context->me.coord));
}
