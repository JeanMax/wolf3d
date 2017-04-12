/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/26 00:14:11 by mc                #+#    #+#             */
/*   Updated: 2017/04/12 16:02:51 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "raycaster.h"

#define MAP_CHAR(MAP, X, Y) (*(*((char **)(MAP) + (int)((Y) / TILE_SIZE)) + (int)((X) / TILE_SIZE)))

#define DOUBLE_PRECISION (1e-6)
#define ZERO(X) ((X) > -DOUBLE_PRECISION * 2 && (X) < DOUBLE_PRECISION * 2)

static void draw_floor_and_sky(SDL_Renderer *renderer)
{
	SDL_Rect rect;

	//TODO: floor is... black

	ft_bzero(&rect, sizeof(rect));
	rect.w = PROJ_WIDTH;
	rect.h = PROJ_HEIGHT / 2;

	SDL_SetRenderDrawColor(renderer, SDL_BLUE);
	SDL_RenderFillRect(renderer, &rect);
}

static e_texture_index pick_texture()
{
	//TODO
	return (TEX_WALL_NORTH);
}

static int pick_stripe(t_point *wall_coord)
{
	if (ZERO(remainder(wall_coord->x, TILE_SIZE))) //vert
		return ((int)ABS(remainder(wall_coord->y, TILE_SIZE)));
	return ((int)ABS(remainder(wall_coord->x, TILE_SIZE)));
}

static void draw_wall(t_context *context, int dst_x, int src_x, \
					  double wall_dist)
{
	SDL_Rect src_rect;
	SDL_Rect dst_rect;

	//TODO: hardcode PROJ_WIDTH?
	if (wall_dist < 0 || ZERO(wall_dist))
		return ;//TODO: catch these weird stuff if they happen
	dst_rect.h = (int)(WALL_HEIGHT / wall_dist * PROJ_WIDTH);
	if (dst_rect.h > PROJ_HEIGHT)
		dst_rect.h = PROJ_HEIGHT;
	else if (dst_rect.h < 1)
		return ; //TODO: catch these weird stuff if they happen
	dst_rect.x = dst_x;
	dst_rect.y = PROJ_HEIGHT / 2 - dst_rect.h / 2;
	dst_rect.w = 1;

	src_rect.x = src_x;
	src_rect.y = 0;
	src_rect.w = 1;
	src_rect.h = TILE_SIZE;

	SDL_RenderCopy(context->renderer, context->textures[pick_texture()],
				   &src_rect, &dst_rect);
	//TODO: catch errors (check all SDL_blahblah call by the way)
}



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
			/* DEBUG(CLR_RED"ZGEG %f/%f\n"CLR_RESET, dst->x, dst->y); */

			/* DEBUG(CLR_GREEN"test>: <%c> in map:%d\n"CLR_RESET, MAP_CHAR(map->ptr, dst->x + 1, dst->y - 1), in_map(map, dst->x + 1, dst->y - 1)); */
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

	/* DEBUG(CLR_RED"PIEG\n"CLR_RESET); */
	return (FALSE);
}

static t_bool check_intersection_v(t_point *dst, double angle, \
								   t_arr *map, t_player *me)
{
	t_point inc;

	//looking straight to the top/bottom
	if (ZERO(mod2pi(angle - M_PI_2)) || ZERO(mod2pi(angle - 3 * M_PI_2)))
		return (FALSE);

	if (LOOKING_RIGHT(angle))
	{
		dst->x = (int)(me->coord.x / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
		inc.x = TILE_SIZE;
	}
	else
	{
		dst->x = (int)(me->coord.x / TILE_SIZE) * TILE_SIZE - DOUBLE_PRECISION;
		inc.x = -TILE_SIZE;
	}

	//looking straight to the left/right
	if (ZERO(mod2pi(angle)) || ZERO(mod2pi(angle - M_PI)) || ZERO(mod2pi(angle - 2 * M_PI)))
	{
		dst->y = me->coord.y;
		inc.y = 0;
	}
	else
	{
		if (LOOKING_DOWN(angle))
		{
			angle = tan(trig_angle(angle));
			inc.y = TILE_SIZE * angle;
			dst->y = me->coord.y + ABS(me->coord.x - dst->x) * angle;
		}
		else
		{
			angle = tan(trig_angle(angle));
			inc.y = -(TILE_SIZE * angle);
			dst->y = me->coord.y - ABS(me->coord.x - dst->x) * angle;
		}
	}

#ifdef DEBUG_MODE
	/* if (!(ZERO(fmod(dst->x, 1))) || !(ZERO(fmod(dst->y, 1)))) */
	/* DEBUG("DST: %f/%f\n", dst->x, dst->y); /\* DEBUG *\/ */

	/* if (ZERO(mod2pi(angle - me->angle))) */
	{
		/* DEBUG(CLR_MAGENTA); */
		/* DEBUG("dstV %f/%f\n", dst->x, dst->y); */
		/* DEBUG("incV %f/%f\n\n", inc.x, inc.y); */
		/* DEBUG(CLR_RESET); */
	}
#endif

	return ((t_bool)get_intersection_coord(map, dst, &inc));
}

static t_bool check_intersection_h(t_point *dst, double angle, \
								   t_arr *map, t_player *me)
{
	t_point inc;

	//looking straight to the left/right
	if (ZERO(mod2pi(angle)) || ZERO(mod2pi(angle - M_PI)) || ZERO(mod2pi(angle - 2 * M_PI)))
		return (FALSE);

	if (LOOKING_DOWN(angle))
	{
		dst->y = (int)(me->coord.y / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
		inc.y = TILE_SIZE;
	}
	else
	{
		dst->y = (int)(me->coord.y / TILE_SIZE) * TILE_SIZE - DOUBLE_PRECISION;
		inc.y = -TILE_SIZE;
	}

	//looking straight to the top/bottom
	if (ZERO(mod2pi(angle - M_PI_2)) || ZERO(mod2pi(angle - 3 * M_PI_2)))
	{
		dst->x = me->coord.x;
		inc.x = 0;
	}
	else
	{
		if (LOOKING_RIGHT(angle))
		{
			angle = tan(trig_angle(angle));
			inc.x = TILE_SIZE / angle;
			dst->x = me->coord.x + ABS(me->coord.y - dst->y) / angle;
		}
		else
		{
			angle = tan(trig_angle(angle));
			inc.x = -(TILE_SIZE / angle);
			dst->x = me->coord.x - ABS(me->coord.y - dst->y) / angle ;
		}
	}

#ifdef DEBUG_MODE
	/* if (ZERO(mod2pi(angle - me->angle))) */
	{
		/* DEBUG(CLR_BLUE); */
		/* DEBUG("dstH %f/%f\n", dst->x, dst->y); */
		/* DEBUG("incH %f/%f\n\n", inc.x, inc.y); */
		/* DEBUG(CLR_RESET); */
	}
#endif

	return (get_intersection_coord(map, dst, &inc));
}

static double correct_fisheye(double angle, double view_angle, double dist)
{
	return (cos(trig_angle(mod2pi(view_angle - angle))) * dist);
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
		wall_dist_v = distance(&context->me.coord, dst, angle);
		wall_dist_h = distance(&context->me.coord, &tmp, angle);
		if (wall_dist_h < wall_dist_v)
		{
			ft_memcpy(dst, &tmp, sizeof(t_point));
			return (wall_dist_h);
		}
		return (wall_dist_v);
	}

	if (dst->x > 0)
	{
		return (distance(&context->me.coord, dst, angle));
	}

	if (tmp.x > 0)
	{
		ft_memcpy(dst, &tmp, sizeof(t_point));
		return (distance(&context->me.coord, &tmp, angle));
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
