/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_player.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/26 04:01:24 by mc                #+#    #+#             */
/*   Updated: 2017/04/18 20:16:28 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sdl.h"

//TODO: same define as in raycaster.c, move that to a header
#define MAP_CHAR(MAP, X, Y) (*(*((char **)(MAP) + (int)((Y) / TILE_SIZE)) + (int)((X) / TILE_SIZE)))

static void adjust_wall_dist(char **map, t_point *me, double angle) //TODO: check before MAPCHAR if coords are in_map
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

//TODO: rename (ft_move_to_x_percent_of_dst_minus_start_from_start() ?)
static void get_dst_coord(t_point *dst, t_point *start, double percent)
{
	start->x += (dst->x - start->x) * percent;
	start->y += (dst->y - start->y) * percent;
}

void move_player(t_context *context, double angle, double distance)
{
	t_polar_point wall;

	wall.angle = angle;
	get_wall(context, &wall);
	if (distance < wall.dist)
	{
		if (wall.dist - distance > MIN_WALL_DIST)
			get_dst_coord(&wall.coord, &context->me.coord,	\
						  distance / wall.dist);
		else
			get_dst_coord(&wall.coord, &context->me.coord,	\
						  (distance - MIN_WALL_DIST) / wall.dist);
	}
	else
		get_dst_coord(&wall.coord, &context->me.coord,					\
					  (wall.dist - MIN_WALL_DIST) / wall.dist);

	adjust_wall_dist((char **)context->map->ptr, &context->me.coord, angle);
}

/**
** update the coordinates of the given player
** @param: the t_player *ME to update
*/
void update_player(t_context *context)
{
	if (context->me.action & A_UP)
		move_player(context, context->me.angle, SPEED_PER_FRAME \
					* ((context->me.status & S_RUN) ? RUN_BONUS : 1));
	if (context->me.action & A_DOWN)
		move_player(context, mod2pi(context->me.angle + M_PI), SPEED_PER_FRAME \
					* ((context->me.status & S_RUN) ? RUN_BONUS : 1));

	if (context->me.action & A_RIGHT)
		move_player(context, mod2pi(context->me.angle - M_PI_2), \
					SPEED_PER_FRAME);
	if (context->me.action & A_LEFT)
		move_player(context, mod2pi(context->me.angle + M_PI_2), \
					SPEED_PER_FRAME);

	if (context->me.action & A_ROLL)
		context->me.angle = mod2pi(context->me.angle + ROLL_PER_FRAME);
	if (context->me.action & A_UNROLL)
		context->me.angle = mod2pi(context->me.angle - ROLL_PER_FRAME);
}
