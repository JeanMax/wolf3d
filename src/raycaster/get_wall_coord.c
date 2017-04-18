/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_wall_coord.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/13 22:04:51 by mc                #+#    #+#             */
/*   Updated: 2017/04/18 00:58:40 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycaster.h"

//TODO: centralise define from raycaster.c

#define MAP_CHAR(MAP, X, Y) (*(*((char **)(MAP) + (int)((Y) / TILE_SIZE)) + (int)((X) / TILE_SIZE)))

#define DOUBLE_PRECISION (1e-6)
#define ZERO(X) ((X) > -DOUBLE_PRECISION * 2 && (X) < DOUBLE_PRECISION * 2)

#define CLR_WALL_NORTH 0xff, 0xff, 0xff
#define CLR_WALL_EAST  0xbb, 0xbb, 0xbb
#define CLR_WALL_SOUTH 0x99, 0x99, 0x99
#define CLR_WALL_WEST  0xdd, 0xdd, 0xdd

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
			/* DEBUG(CLR_RED"ZGEG %f/%f", dst->x, dst->y); */

			/* DEBUG(CLR_GREEN"test>: <%c> in map:%d", MAP_CHAR(map->ptr, dst->x + 1, dst->y - 1), in_map(map, dst->x + 1, dst->y - 1)); */
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

	/* DEBUG(CLR_RED"PIEG"); */
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

/* //TODO: move? */
/* static void color_wall(t_context *context, t_bool is_vertical, double angle) */
/* { */
/* 	if (is_vertical) */
/* 	{ */
/* 		if (LOOKING_RIGHT(angle)) */
/* 			SDL_SetTextureColorMod(context->textures[TEX_WALL], CLR_WALL_WEST); */
/* 		else */
/* 			SDL_SetTextureColorMod(context->textures[TEX_WALL], CLR_WALL_EAST); */
/* 	} */
/* 	else */
/* 	{ */
/* 		if (LOOKING_DOWN(angle)) */
/* 			SDL_SetTextureColorMod(context->textures[TEX_WALL], CLR_WALL_NORTH); */
/* 		else */
/* 			SDL_SetTextureColorMod(context->textures[TEX_WALL], CLR_WALL_SOUTH); */
/* 	} */
/* } */

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
		if (wall_dist_h < wall_dist_v) //horzi
		{
			/* color_wall(context, FALSE, angle); */
			ft_memcpy(dst, &tmp, sizeof(t_point));
			return (wall_dist_h);
		}
		//vert
		/* color_wall(context, TRUE, angle); */
		return (wall_dist_v);
	}

	if (dst->x > 0) //vert
	{
		/* color_wall(context, TRUE, angle); */
		return (distance(&context->me.coord, dst, angle));
	}

	if (tmp.x > 0) //hori
	{
		/* color_wall(context, FALSE, angle); */
		ft_memcpy(dst, &tmp, sizeof(t_point));
		return (distance(&context->me.coord, &tmp, angle));
	}

	ft_bzero(dst, sizeof(t_point));
	return (-1); //TODO: throw error?
	//this should never happen if the map is bordered with walls
}
