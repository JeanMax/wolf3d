/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_wall.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/13 22:04:51 by mc                #+#    #+#             */
/*   Updated: 2017/09/12 18:08:51 by mcanal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** norme, tired: w == wall, c == context... sorry!
*/

#include "raycaster.h"

static t_bool	get_intersection_coord(t_arr *map, t_point *dst, t_point *inc)
{
	while (in_map(map, dst->x, dst->y))
	{
		if (MAP_CHAR(map->ptr, dst->x, dst->y) == WALL)
			return (TRUE);
		if (ZERO(remainder(dst->x, TILE_SIZE)) \
			&& ZERO(remainder(dst->y, TILE_SIZE)))
		{
			if (in_map(map, dst->x + 1, dst->y - 1) \
				&& (MAP_CHAR(map->ptr, dst->x + 1, dst->y - 1) == WALL))
				return (TRUE);
			if (in_map(map, dst->x - 1, dst->y + 1) \
				&& (MAP_CHAR(map->ptr, dst->x - 1, dst->y + 1) == WALL))
				return (TRUE);
			if (in_map(map, dst->x - 1, dst->y - 1) \
				&& (MAP_CHAR(map->ptr, dst->x - 1, dst->y - 1) == WALL))
				return (TRUE);
			if (in_map(map, dst->x + 1, dst->y + 1) \
				&& (MAP_CHAR(map->ptr, dst->x + 1, dst->y + 1) == WALL))
				return (TRUE);
		}
		dst->x += inc->x;
		dst->y += inc->y;
	}
	return (FALSE);
}

static t_bool	check_intersection_v(t_context *c, t_polar_point *w)
{
	t_point inc;

	w->coord.x = (int)(c->me.coord.x / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
	inc.x = TILE_SIZE;
	if (LOOKING_LEFT(w->angle))
	{
		w->coord.x -= TILE_SIZE + DOUBLE_PRECISION;
		inc.x = -TILE_SIZE;
	}
	if (LOOKING_HORI(w->angle))
	{
		w->coord.y = c->me.coord.y;
		inc.y = 0;
		return (get_intersection_coord(c->map, &w->coord, &inc));
	}
	inc.y = c->tan_table[trig_angle(w->angle)];
	if (LOOKING_DOWN(w->angle))
	{
		w->coord.y = c->me.coord.y + ABS(c->me.coord.x - w->coord.x) * inc.y;
		inc.y *= TILE_SIZE;
		return (get_intersection_coord(c->map, &w->coord, &inc));
	}
	w->coord.y = c->me.coord.y - ABS(c->me.coord.x - w->coord.x) * inc.y;
	inc.y *= -TILE_SIZE;
	return (get_intersection_coord(c->map, &w->coord, &inc));
}

static t_bool	check_intersection_h(t_context *c, t_polar_point *w)
{
	t_point inc;

	w->coord.y = (int)(c->me.coord.y / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
	inc.y = TILE_SIZE;
	if (LOOKING_UP(w->angle))
	{
		w->coord.y -= TILE_SIZE + DOUBLE_PRECISION;
		inc.y = -TILE_SIZE;
	}
	if (LOOKING_VERT(w->angle))
	{
		w->coord.x = c->me.coord.x;
		inc.x = 0;
		return (get_intersection_coord(c->map, &w->coord, &inc));
	}
	inc.x = c->tan_table[trig_angle(w->angle)];
	if (LOOKING_RIGHT(w->angle))
	{
		w->coord.x = c->me.coord.x + ABS(c->me.coord.y - w->coord.y) / inc.x;
		inc.x = TILE_SIZE / inc.x;
		return (get_intersection_coord(c->map, &w->coord, &inc));
	}
	w->coord.x = c->me.coord.x - ABS(c->me.coord.y - w->coord.y) / inc.x;
	inc.x = -(TILE_SIZE / inc.x);
	return (get_intersection_coord(c->map, &w->coord, &inc));
}

static t_bool	pick_closest_wall(t_polar_point *w, t_polar_point *w_h, \
								t_point *me, t_context *c)
{
	if (w_h->coord.x > 0 && w->coord.x > 0)
	{
		w->dist = distance(me, &w->coord, w->angle, c);
		w_h->dist = distance(me, &w_h->coord, w->angle, c);
		if (w_h->dist < w->dist)
			ft_memcpy(w, w_h, sizeof(t_polar_point));
		return (w->dist > 0);
	}
	if (w->coord.x > 0)
		return ((w->dist = distance(me, &w->coord, w->angle, c)) > 0);
	if (w_h->coord.x > 0)
	{
		ft_memcpy(&w->coord, &w_h->coord, sizeof(t_point));
		return ((w->dist = distance(me, &w_h->coord, w->angle, c)) > 0);
	}
	w->dist = -1.;
	return (FALSE);
}

/*
** store coordinates and distance of the wall in sight in W
** @param: starting from CONTEXT->me.coord at the given W.ANGLE
** @return: FALSE if no wall found
*/

t_bool			get_wall(t_context *c, t_polar_point *w)
{
	t_polar_point w_h;

	w->coord.x = -1;
	w_h.coord.x = -1;
	w_h.angle = w->angle;
	if (!LOOKING_VERT(w->angle))
		check_intersection_v(c, w);
	if (!LOOKING_HORI(w->angle))
		check_intersection_h(c, &w_h);
	return (pick_closest_wall(w, &w_h, &c->me.coord, c));
}
