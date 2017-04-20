/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coord_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/10 22:06:25 by mc                #+#    #+#             */
/*   Updated: 2017/04/20 21:03:34 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** a few helpers functions for coordinates/angles manipulation
*/

#include "raycaster.h"

int mod2pi(int angle)
{
	if (angle < 0)
		angle += 2 * PI;
	else if (angle >= 2 * PI)
		angle -= 2 * PI;
	return (angle);
}

int trig_angle(int angle)
{
	if (LOOKING_DOWN(angle))
	{
		if (LOOKING_RIGHT(angle))
			return (2 * PI - angle);
		return (angle - PI);
	}
	if (LOOKING_LEFT(angle))
		return (PI - angle);
	return (angle);
}

double distance(t_point *a, t_point *b, int angle, t_context *context)
{
	if (ZERO(a->x - b->x))
		return (ABS(a->y - b->y));
	if (ZERO(a->y - b->y))
		return (ABS(a->x - b->x));
	if (angle == PI_2)
		return (ABS(a->y - b->y));
	return (ABS(a->x - b->x) / context->cos_table[trig_angle(angle)]);
}

t_bool in_map(t_arr *map, double x, double y)
{
	return (x >= 0							\
			&& y >= 0						\
			&& x < map->length * TILE_SIZE	\
			&& y < map->length * TILE_SIZE);
}
