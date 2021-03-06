/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_get_wall_coord.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/09 13:55:23 by mc                #+#    #+#             */
/*   Updated: 2017/04/20 21:26:03 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d_test.h"

#define _DOUBLE_PRECISION (1e-5)
#define _ZERO(X) ((X) > -_DOUBLE_PRECISION && (X) < _DOUBLE_PRECISION)

static void check_point(t_context *context, t_point *ctrl, double ctrl_dist)
{
	t_polar_point wall;

	bzero(&wall, sizeof(t_point));
	wall.angle = context->me.angle;

	MU_ASSERT(get_wall(context, &wall),		\
			  "get_wall failed "CLR_BLUE"\n(wall: %f/%f at %f) "CLR_MAGENTA"\n(me:   %f/%f at %d)"CLR_RESET, \
			  wall.coord.x, wall.coord.y, wall.dist,			\
			  context->me.coord.x, context->me.coord.y, context->me.angle);

	MU_ASSERT(_ZERO(wall.dist - ctrl_dist),						\
			  "dist: "CLR_RED"%f != %f "CLR_BLUE"\n(wall: %f/%f) "CLR_MAGENTA"\n(me:   %f/%f at %d)"CLR_RESET, \
			  wall.dist, ctrl_dist, wall.coord.x, wall.coord.y,				\
			  context->me.coord.x, context->me.coord.y, context->me.angle);

	MU_ASSERT(_ZERO(wall.coord.x - ctrl->x), \
			  "x: "CLR_RED"%f != %f "CLR_BLUE"\n(wall: %f/%f) "CLR_MAGENTA"\n(ctrl: %f/%f) "CLR_CYAN"\n(me:   %f/%f at %d)"CLR_RESET, \
			  wall.coord.x, ctrl->x, wall.coord.x, wall.coord.y,				\
			  ctrl->x, ctrl->y, \
			  context->me.coord.x, context->me.coord.y, context->me.angle);

	MU_ASSERT(_ZERO(wall.coord.y - ctrl->y),							\
			  "y: "CLR_RED"%f != %f "CLR_BLUE"\n(wall: %f/%f) "CLR_MAGENTA"\n(ctrl: %f/%f) "CLR_CYAN"\n(me:   %f/%f at %d)"CLR_RESET, \
			  wall.coord.y, ctrl->y, wall.coord.x, wall.coord.y,				\
			  ctrl->x, ctrl->y, \
			  context->me.coord.x, context->me.coord.y, context->me.angle);
}

void test_get_wall_coord_3x3()
{
	t_context context;
	int i;
	char **map;

	bzero(&context, sizeof(t_context));
	init_tables(&context);
	generate_maze(3, &context);

	map = context.map->ptr;

	context.me.coord.x = 1.5 * TILE_SIZE;
	context.me.coord.y = 1.5 * TILE_SIZE;

	t_point ctrl_a[] = {
		{2.  * TILE_SIZE, 1.5 * TILE_SIZE},
		{1.5 * TILE_SIZE, 1.  * TILE_SIZE},
		{1.  * TILE_SIZE, 1.5 * TILE_SIZE},
		{1.5 * TILE_SIZE, 2.  * TILE_SIZE},
	};
	for (i = 0, context.me.angle = 0; context.me.angle < 2 * PI; context.me.angle += PI_2, i++) {
		check_point(&context, &ctrl_a[i], TILE_SIZE / 2);
	}

	t_point ctrl_b[] = {
		{2.  * TILE_SIZE, 1.  * TILE_SIZE},
		{1.  * TILE_SIZE, 1.  * TILE_SIZE},
		{1.  * TILE_SIZE, 2.  * TILE_SIZE},
		{2.  * TILE_SIZE, 2.  * TILE_SIZE},
	};
	for (i = 0, context.me.angle = PI_4; context.me.angle < 2 * PI; context.me.angle += PI_2, i++) {
		check_point(&context, &ctrl_b[i], TILE_SIZE / 2 * M_SQRT2);
	}

	double wall_dist = (TILE_SIZE / 2) / cos(M_PI / 8);
	double adj = sqrt(pow(wall_dist, 2) - pow(TILE_SIZE / 2, 2));
	t_point ctrl_c[] = {
		{2.  * TILE_SIZE,       1.5 * TILE_SIZE - adj},
		{1.5 * TILE_SIZE + adj, 1.  * TILE_SIZE},
		{1.5 * TILE_SIZE - adj, 1.  * TILE_SIZE},
		{1.  * TILE_SIZE,       1.5 * TILE_SIZE - adj},
		{1.  * TILE_SIZE,       1.5 * TILE_SIZE + adj},
		{1.5 * TILE_SIZE - adj, 2.  * TILE_SIZE},
		{1.5 * TILE_SIZE + adj, 2.  * TILE_SIZE},
		{2.  * TILE_SIZE,       1.5 * TILE_SIZE + adj},
	};
	for (i = 0, context.me.angle = PI / 8; context.me.angle < 2 * PI; context.me.angle += PI_4, i++) {
		check_point(&context, &ctrl_c[i], wall_dist);
	}

	map[0][0] = EMPTY;
	map[0][2] = EMPTY;
	map[2][0] = EMPTY;
	map[2][2] = EMPTY;
	for (i = 0, context.me.angle = PI_4; context.me.angle < 2 * PI; context.me.angle += PI_2, i++) {
		check_point(&context, &ctrl_b[i], TILE_SIZE / 2 * M_SQRT2);
	}

	ft_arrdel(&context.map);
}

void test_get_wall_coord_5x5()
{
	t_context context;
	int i;
	char map_1[] = {WALL, WALL,  WALL,  WALL,  WALL, 0};
	char map_2[] = {WALL, EMPTY, EMPTY, EMPTY, WALL, 0};
	char **map;

	bzero(&context, sizeof(t_context));
	init_tables(&context);
	generate_maze(5, &context);

	map = context.map->ptr;
	strcpy(*map, map_1);
	strcpy(*(map + 1), map_2);
	strcpy(*(map + 2), map_2);
	strcpy(*(map + 3), map_2);
	strcpy(*(map + 4), map_1);

	context.me.coord.x = 2.5 * TILE_SIZE;
	context.me.coord.y = 2.5 * TILE_SIZE;

	t_point ctrl_a[] = {
		{4.  * TILE_SIZE, 2.5 * TILE_SIZE},
		{2.5 * TILE_SIZE, 1.  * TILE_SIZE},
		{1.  * TILE_SIZE, 2.5 * TILE_SIZE},
		{2.5 * TILE_SIZE, 4.  * TILE_SIZE},
	};
	for (i = 0, context.me.angle = 0; context.me.angle < 2 * PI; context.me.angle += PI_2, i++) {
		check_point(&context, &ctrl_a[i], 1.5 * TILE_SIZE);
	}

	t_point ctrl_b[] = {
		{4.  * TILE_SIZE, 1.  * TILE_SIZE},
		{1.  * TILE_SIZE, 1.  * TILE_SIZE},
		{1.  * TILE_SIZE, 4.  * TILE_SIZE},
		{4.  * TILE_SIZE, 4.  * TILE_SIZE},
	};
	for (i = 0, context.me.angle = PI_4; context.me.angle < 2 * PI; context.me.angle += PI_2, i++) {
		check_point(&context, &ctrl_b[i], 1.5 * TILE_SIZE * M_SQRT2);
	}

	double wall_dist = (1.5 * TILE_SIZE) / cos(M_PI / 8);
	double adj = sqrt(pow(wall_dist, 2) - pow(1.5 * TILE_SIZE, 2));
	t_point ctrl_c[] = {
		{4.  * TILE_SIZE,       2.5 * TILE_SIZE - adj},
		{2.5 * TILE_SIZE + adj, 1.  * TILE_SIZE},
		{2.5 * TILE_SIZE - adj, 1.  * TILE_SIZE},
		{1.  * TILE_SIZE,       2.5 * TILE_SIZE - adj},
		{1.  * TILE_SIZE,       2.5 * TILE_SIZE + adj},
		{2.5 * TILE_SIZE - adj, 4.  * TILE_SIZE},
		{2.5 * TILE_SIZE + adj, 4.  * TILE_SIZE},
		{4.  * TILE_SIZE,       2.5 * TILE_SIZE + adj},
	};
	for (i = 0, context.me.angle = PI / 8; context.me.angle < 2 * PI; context.me.angle += PI_4, i++) {
		check_point(&context, &ctrl_c[i], wall_dist);
	}

	map[0][0] = EMPTY;
	map[0][4] = EMPTY;
	map[4][0] = EMPTY;
	map[4][4] = EMPTY;
	for (i = 0, context.me.angle = PI_4; context.me.angle < 2 * PI; context.me.angle += PI_2, i++) {
		check_point(&context, &ctrl_b[i], 1.5 * TILE_SIZE * M_SQRT2);
	}

	ft_arrdel(&context.map);
}
