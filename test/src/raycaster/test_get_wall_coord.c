/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_get_wall_coord.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/09 13:55:23 by mc                #+#    #+#             */
/*   Updated: 2017/04/09 14:27:03 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d_test.h"

#define DOUBLE_PRECISION 1e-6
/* #define ZERO(X) ((X) > -DOUBLE_PRECISION && (X) < DOUBLE_PRECISION) */
#define ZERO(X) ((X) < DOUBLE_PRECISION)

void test_get_wall_coord()
{
	double wall_dist;
	t_point wall;
	t_context context;

	bzero(&context, sizeof(t_context));
	generate_maze(3, &context);

	context.me.coord.x = 1.5 * TILE_SIZE;
	context.me.coord.y = 1.5 * TILE_SIZE;

	for (context.me.angle = 0; context.me.angle < 2 * M_PI; context.me.angle += M_PI_2) {
		bzero(&wall, sizeof(t_point));

		MU_ASSERT((wall_dist = get_wall_coord(&wall, &context, context.me.angle)) > 0, \
				  "dist (%f) negative "CLR_BLUE"(wall: %f/%f) "CLR_MAGENTA"(me: %f/%f at %f rad)"CLR_RESET, \
				  wall_dist, wall.x, wall.y, \
				  context.me.coord.x, context.me.coord.y, context.me.angle);

		MU_ASSERT(ZERO(wall_dist - (double)TILE_SIZE / 2),						\
				  "dist: "CLR_RED"%f != %f "CLR_BLUE"(wall: %f/%f) "CLR_MAGENTA"(me: %f/%f at %f rad)"CLR_RESET, \
				  wall_dist, (double)TILE_SIZE / 2, wall.x, wall.y,				\
				  context.me.coord.x, context.me.coord.y, context.me.angle);
	}

	for (context.me.angle = M_PI_4; context.me.angle < 2 * M_PI; context.me.angle += M_PI_2) {
		bzero(&wall, sizeof(t_point));

		MU_ASSERT((wall_dist = get_wall_coord(&wall, &context, context.me.angle)) > 0, \
				  "dist (%f) negative "CLR_BLUE"(wall: %f/%f) "CLR_MAGENTA"(me: %f/%f at %f rad)"CLR_RESET, \
				  wall_dist, wall.x, wall.y, \
				  context.me.coord.x, context.me.coord.y, context.me.angle);

		MU_ASSERT(ZERO(wall_dist - TILE_SIZE / 2 * M_SQRT2),						\
				  "dist: "CLR_RED"%f != %f "CLR_BLUE"(wall: %f/%f) "CLR_MAGENTA"(me: %f/%f at %f rad)"CLR_RESET, \
				  wall_dist, TILE_SIZE / 2 * M_SQRT2, wall.x, wall.y,	\
				  context.me.coord.x, context.me.coord.y, context.me.angle);
	}

	ft_arrdel(&context.map);
}
