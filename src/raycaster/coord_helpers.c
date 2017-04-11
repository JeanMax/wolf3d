/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coord_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/10 22:06:25 by mc                #+#    #+#             */
/*   Updated: 2017/04/11 00:32:19 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycaster.h"

#define DOUBLE_PRECISION (1e-6)
#define ZERO(X) ((X) > -DOUBLE_PRECISION * 2 && (X) < DOUBLE_PRECISION * 2)

double mod2pi(double angle)
{
	if (angle < 0)
		angle = fmod(2 * M_PI + angle, 2 * M_PI);
	else if (angle >= 2 * M_PI)
		angle = fmod(angle, 2 * M_PI);
	return (angle);
}

double trig_angle(double angle)
{
	if (LOOKING_DOWN(angle))
	{
		if (LOOKING_RIGHT(angle))
			return (2 * M_PI - angle);
		else
			return (angle - M_PI);
	}
	else
	{
		if (LOOKING_LEFT(angle))
			return (M_PI - angle);
	}

	return (angle);
}

double distance(t_point *a, t_point *b, double angle)
{
	angle = cos(trig_angle(angle));
	if (ZERO(angle) || ZERO(a->x - b->x))
		return (sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2)));
	return (ABS(a->x - b->x) / angle);
}

t_bool in_map(t_arr *map, double x, double y)
{
	return (x >= 0							\
			&& y >= 0						\
			&& x < map->length * TILE_SIZE	\
			&& y < map->length * TILE_SIZE);
}
