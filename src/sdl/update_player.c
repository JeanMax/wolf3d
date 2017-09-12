/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_player.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/26 04:01:24 by mc                #+#    #+#             */
/*   Updated: 2017/09/12 18:48:26 by mcanal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sdlux.h"

static void	adjust_wall_dist(char **map, t_point *me, int angle)
{
	if (LOOKING_RIGHT(angle))
	{
		if (MAP_CHAR(map, me->x + MIN_WALL_DIST, me->y) == WALL)
			me->x = (double)((int)(me->x / TILE_SIZE + 1) * TILE_SIZE) \
				- MIN_WALL_DIST;
	}
	else
	{
		if (MAP_CHAR(map, me->x - MIN_WALL_DIST, me->y) == WALL)
			me->x = (double)((int)(me->x / TILE_SIZE) * TILE_SIZE) \
				+ MIN_WALL_DIST;
	}
	if (LOOKING_DOWN(angle))
	{
		if (MAP_CHAR(map, me->x, me->y + MIN_WALL_DIST) == WALL)
			me->y = (double)((int)(me->y / TILE_SIZE + 1) * TILE_SIZE) \
				- MIN_WALL_DIST;
	}
	else
	{
		if (MAP_CHAR(map, me->x, me->y - MIN_WALL_DIST) == WALL)
			me->y = (double)((int)(me->y / TILE_SIZE) * TILE_SIZE) \
				+ MIN_WALL_DIST;
	}
}

static void	get_dst_coord(t_point *dst, t_point *start, double percent)
{
	start->x += (dst->x - start->x) * percent;
	start->y += (dst->y - start->y) * percent;
}

void		move_player(t_context *context, int angle, double distance)
{
	t_polar_point wall;

	wall.angle = angle;
	get_wall(context, &wall);
	if (distance < wall.dist)
	{
		if (wall.dist - distance > MIN_WALL_DIST)
			get_dst_coord(&wall.coord, &context->me.coord, \
						distance / wall.dist);
		else
			get_dst_coord(&wall.coord, &context->me.coord, \
						(distance - MIN_WALL_DIST) / wall.dist);
	}
	else
		get_dst_coord(&wall.coord, &context->me.coord, \
					(wall.dist - MIN_WALL_DIST) / wall.dist);
	adjust_wall_dist((char **)context->map->ptr, &context->me.coord, angle);
}

/*
** update the coordinates of the given player
** @param: the t_player *ME to update
*/

void		update_player(t_context *context)
{
	double distance;

	distance = SPEED_PER_FRAME;
	if (((context->me.action & A_UP) || (context->me.action & A_DOWN))
			&& context->me.status & S_RUN)
		distance *= RUN_BONUS;
	if (context->me.action & A_UP)
		move_player(context, context->me.angle, distance);
	if (context->me.action & A_DOWN)
		move_player(context, mod2pi(context->me.angle + PI), distance);
	if (context->me.action & A_RIGHT)
		move_player(context, mod2pi(context->me.angle - PI_2), distance);
	if (context->me.action & A_LEFT)
		move_player(context, mod2pi(context->me.angle + PI_2), distance);
	if (context->me.action & A_ROLL)
		context->me.angle = mod2pi(context->me.angle + ROLL_PER_FRAME);
	if (context->me.action & A_UNROLL)
		context->me.angle = mod2pi(context->me.angle - ROLL_PER_FRAME);
}
