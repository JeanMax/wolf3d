/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/10 23:47:20 by mc                #+#    #+#             */
/*   Updated: 2017/04/18 00:58:22 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d_test.h"

#define DOUBLE_PRECISION (1e-5)
#define ZERO(X) ((X) > -DOUBLE_PRECISION && (X) < DOUBLE_PRECISION)

void test_move_player()
{
	t_context context;
	int i;
	char **map;

	bzero(&context, sizeof(t_context));
	generate_maze(3, &context);

	map = context.map->ptr;
	/* map[0][0] = EMPTY; */
	/* map[0][2] = EMPTY; */
	/* map[2][0] = EMPTY; */
	/* map[2][2] = EMPTY; */

	t_point ctrl_a[] = {
		{1.75  * TILE_SIZE, 1.5 * TILE_SIZE},
		{1.5 * TILE_SIZE, 1.25  * TILE_SIZE},
		{1.25  * TILE_SIZE, 1.5 * TILE_SIZE},
		{1.5 * TILE_SIZE, 1.75  * TILE_SIZE},
	};
	for (i = 0, context.me.angle = 0; context.me.angle < 2 * M_PI; context.me.angle += M_PI_2, i++) {

		context.me.coord.x = 1.5 * TILE_SIZE;
		context.me.coord.y = 1.5 * TILE_SIZE;
		move_player(&context, context.me.angle, TILE_SIZE / 4);

		MU_ASSERT(ZERO(context.me.coord.x - ctrl_a[i].x),					\
				  "x: "CLR_RED"%f != %f "CLR_BLUE"\n(me: %f/%f at %f rad) "CLR_MAGENTA"\n(ctrl: %f/%f)", \
				  context.me.coord.x, ctrl_a[i].x,						\
				  context.me.coord.x, context.me.coord.y, context.me.angle, \
				  ctrl_a[i].x, ctrl_a[i].y);

		MU_ASSERT(ZERO(context.me.coord.y - ctrl_a[i].y),					\
				  "y: "CLR_RED"%f != %f "CLR_BLUE"\n(me: %f/%f at %f rad) "CLR_MAGENTA"\n(ctrl: %f/%f)", \
				  context.me.coord.y, ctrl_a[i].y, \
				  context.me.coord.x, context.me.coord.y, context.me.angle, \
				  ctrl_a[i].x, ctrl_a[i].y);
	}


	t_point ctrl_b[] = {
		{2.  * TILE_SIZE - MIN_WALL_DIST, 1.5 * TILE_SIZE},
		{1.5 * TILE_SIZE,                 1.  * TILE_SIZE + MIN_WALL_DIST},
		{1.  * TILE_SIZE + MIN_WALL_DIST, 1.5 * TILE_SIZE},
		{1.5 * TILE_SIZE,                 2.  * TILE_SIZE - MIN_WALL_DIST},
	};
	for (i = 0, context.me.angle = 0; context.me.angle < 2 * M_PI; context.me.angle += M_PI_2, i++) {

		context.me.coord.x = 1.5 * TILE_SIZE;
		context.me.coord.y = 1.5 * TILE_SIZE;
		move_player(&context, context.me.angle, TILE_SIZE * 4);

		MU_ASSERT(ZERO(context.me.coord.x - ctrl_b[i].x),					\
				  "x: "CLR_RED"%f != %f "CLR_BLUE"\n(me: %f/%f at %f rad) "CLR_MAGENTA"\n(ctrl: %f/%f)", \
				  context.me.coord.x, ctrl_b[i].x,						\
				  context.me.coord.x, context.me.coord.y, context.me.angle, \
				  ctrl_b[i].x, ctrl_b[i].y);

		MU_ASSERT(ZERO(context.me.coord.y - ctrl_b[i].y),					\
				  "y: "CLR_RED"%f != %f "CLR_BLUE"\n(me: %f/%f at %f rad) "CLR_MAGENTA"\n(ctrl: %f/%f)", \
				  context.me.coord.y, ctrl_b[i].y, \
				  context.me.coord.x, context.me.coord.y, context.me.angle, \
				  ctrl_b[i].x, ctrl_b[i].y);


		context.me.coord.x = 1.5 * TILE_SIZE;
		context.me.coord.y = 1.5 * TILE_SIZE;
		move_player(&context, context.me.angle, TILE_SIZE / 2 - MIN_WALL_DIST / 2);

		MU_ASSERT(ZERO(context.me.coord.x - ctrl_b[i].x),					\
				  "x: "CLR_RED"%f != %f "CLR_BLUE"\n(me: %f/%f at %f rad) "CLR_MAGENTA"\n(ctrl: %f/%f)", \
				  context.me.coord.x, ctrl_b[i].x,						\
				  context.me.coord.x, context.me.coord.y, context.me.angle, \
				  ctrl_b[i].x, ctrl_b[i].y);

		MU_ASSERT(ZERO(context.me.coord.y - ctrl_b[i].y),					\
				  "y: "CLR_RED"%f != %f "CLR_BLUE"\n(me: %f/%f at %f rad) "CLR_MAGENTA"\n(ctrl: %f/%f)", \
				  context.me.coord.y, ctrl_b[i].y, \
				  context.me.coord.x, context.me.coord.y, context.me.angle, \
				  ctrl_b[i].x, ctrl_b[i].y);
	}


	t_point ctrl_c[] = {
		{2.  * TILE_SIZE - MIN_WALL_DIST, 1.  * TILE_SIZE + MIN_WALL_DIST},
		{1.  * TILE_SIZE + MIN_WALL_DIST, 1.  * TILE_SIZE + MIN_WALL_DIST},
		{1.  * TILE_SIZE + MIN_WALL_DIST, 2.  * TILE_SIZE - MIN_WALL_DIST},
		{2.  * TILE_SIZE - MIN_WALL_DIST, 2.  * TILE_SIZE - MIN_WALL_DIST},
	};
	for (i = 0, context.me.angle = M_PI_4; context.me.angle < 2 * M_PI; context.me.angle += M_PI_2, i++) {

		context.me.coord.x = 1.5 * TILE_SIZE;
		context.me.coord.y = 1.5 * TILE_SIZE;
		move_player(&context, context.me.angle, TILE_SIZE * 4);

		MU_ASSERT(ZERO(context.me.coord.x - ctrl_c[i].x),				\
				  "x: "CLR_RED"%f != %f "CLR_BLUE"\n(me: %f/%f at %f rad) "CLR_MAGENTA"\n(ctrl: %f/%f)", \
				  context.me.coord.x, ctrl_c[i].x,						\
				  context.me.coord.x, context.me.coord.y, context.me.angle, \
				  ctrl_c[i].x, ctrl_c[i].y);

		MU_ASSERT(ZERO(context.me.coord.y - ctrl_c[i].y),					\
				  "y: "CLR_RED"%f != %f "CLR_BLUE"\n(me: %f/%f at %f rad) "CLR_MAGENTA"\n(ctrl: %f/%f)", \
				  context.me.coord.y, ctrl_c[i].y, \
				  context.me.coord.x, context.me.coord.y, context.me.angle, \
				  ctrl_c[i].x, ctrl_c[i].y);
	}

	ft_arrdel(&context.map);
}
