/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_get_floor_coord.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/09 13:55:23 by mc                #+#    #+#             */
/*   Updated: 2017/04/14 19:41:01 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d_test.h"

#define DOUBLE_PRECISION (1e-5)
#define ZERO(X) ((X) > -DOUBLE_PRECISION && (X) < DOUBLE_PRECISION)

#define RADTODEG(angle) ((int)(angle / M_PI * 180))

static void check_point(t_context *context, t_point *ctrl, double ctrl_dist, double vert_angle)
{
	t_point floor;
	double floor_dist;

	bzero(&floor, sizeof(t_point));

	MU_ASSERT((floor_dist = get_floor_coord(&floor, context, context->me.angle, vert_angle)) > 0, \
			  "dist (%f) negative "CLR_BLUE"\n(floor: %f/%f) "CLR_MAGENTA"\n(me:    %f/%f at %d/%d°)"CLR_RESET, \
			  floor_dist / TILE_SIZE, floor.x / TILE_SIZE, floor.y / TILE_SIZE, \
			  context->me.coord.x / TILE_SIZE, context->me.coord.y / TILE_SIZE, \
			  RADTODEG(context->me.angle), RADTODEG(vert_angle));

	MU_ASSERT(ZERO(floor_dist - ctrl_dist),						\
			  "dist: "CLR_RED"%f != %f "CLR_BLUE"\n(floor: %f/%f) "CLR_MAGENTA"\n(me:    %f/%f at %d/%d°)"CLR_RESET, \
			  floor_dist / TILE_SIZE, ctrl_dist / TILE_SIZE, floor.x / TILE_SIZE, floor.y / TILE_SIZE,				\
			  context->me.coord.x / TILE_SIZE, context->me.coord.y / TILE_SIZE, \
			  RADTODEG(context->me.angle), RADTODEG(vert_angle));

	MU_ASSERT(ZERO(floor.x - ctrl->x), \
			  "x: "CLR_RED"%f != %f "CLR_BLUE"\n(floor: %f/%f) "CLR_MAGENTA"\n(ctrl:  %f/%f) "CLR_CYAN"\n(me:    %f/%f at %d/%d°)"CLR_RESET, \
			  floor.x / TILE_SIZE, ctrl->x / TILE_SIZE, floor.x / TILE_SIZE, floor.y / TILE_SIZE,				\
			  ctrl->x / TILE_SIZE, ctrl->y / TILE_SIZE, \
			  context->me.coord.x / TILE_SIZE, context->me.coord.y / TILE_SIZE, \
			  RADTODEG(context->me.angle), RADTODEG(vert_angle));

	MU_ASSERT(ZERO(floor.y - ctrl->y),							\
			  "y: "CLR_RED"%f != %f "CLR_BLUE"\n(floor: %f/%f) "CLR_MAGENTA"\n(ctrl:  %f/%f) "CLR_CYAN"\n(me:    %f/%f at %d/%d°)"CLR_RESET, \
			  floor.y / TILE_SIZE, ctrl->y / TILE_SIZE, floor.x / TILE_SIZE, floor.y / TILE_SIZE,				\
			  ctrl->x / TILE_SIZE, ctrl->y / TILE_SIZE, \
			  context->me.coord.x / TILE_SIZE, context->me.coord.y / TILE_SIZE, \
			  RADTODEG(context->me.angle), RADTODEG(vert_angle));
}

void test_get_floor_coord()
{
	t_context context;
	int i;
	char **map;

	bzero(&context, sizeof(t_context));
	generate_maze(3, &context);

	map = context.map->ptr;

	context.me.coord.x = 1.5 * TILE_SIZE;
	context.me.coord.y = 1.5 * TILE_SIZE;

	double vert_angle = M_PI_4;
	double floor_dist = (double)PLAYER_HEIGHT * tan(vert_angle);
	t_point ctrl_a[] = {
		{1.5 * TILE_SIZE + floor_dist, 1.5 * TILE_SIZE},
		{1.5 * TILE_SIZE,              1.5 * TILE_SIZE - floor_dist},
		{1.5 * TILE_SIZE - floor_dist, 1.5 * TILE_SIZE},
		{1.5 * TILE_SIZE,              1.5 * TILE_SIZE + floor_dist},
	};
	for (i = 0, context.me.angle = 0; context.me.angle < 2 * M_PI; context.me.angle += M_PI_2, i++) {
		check_point(&context, &ctrl_a[i], floor_dist, vert_angle);
	}

	vert_angle = 2. / 3. * M_PI_2;
	floor_dist = (double)PLAYER_HEIGHT * tan(vert_angle);
	t_point ctrl_b[] = {
		{1.5 * TILE_SIZE + floor_dist, 1.5 * TILE_SIZE},
		{1.5 * TILE_SIZE,              1.5 * TILE_SIZE - floor_dist},
		{1.5 * TILE_SIZE - floor_dist, 1.5 * TILE_SIZE},
		{1.5 * TILE_SIZE,              1.5 * TILE_SIZE + floor_dist},
	};
	for (i = 0, context.me.angle = 0; context.me.angle < 2 * M_PI; context.me.angle += M_PI_2, i++) {
		check_point(&context, &ctrl_b[i], floor_dist, vert_angle);
	}


	/* t_point ctrl_b[] = { */
	/* 	{2.  * TILE_SIZE, 1.  * TILE_SIZE}, */
	/* 	{1.  * TILE_SIZE, 1.  * TILE_SIZE}, */
	/* 	{1.  * TILE_SIZE, 2.  * TILE_SIZE}, */
	/* 	{2.  * TILE_SIZE, 2.  * TILE_SIZE}, */
	/* }; */
	/* for (i = 0, context.me.angle = M_PI_4; context.me.angle < 2 * M_PI; context.me.angle += M_PI_2, i++) { */
	/* 	check_point(&context, &ctrl_b[i], TILE_SIZE / 2 * M_SQRT2); */
	/* } */

	/* double floor_dist = (TILE_SIZE / 2) / cos(M_PI / 8); */
	/* double adj = sqrt(pow(floor_dist, 2) - pow(TILE_SIZE / 2, 2)); */
	/* t_point ctrl_c[] = { */
	/* 	{2.  * TILE_SIZE,       1.5 * TILE_SIZE - adj}, */
	/* 	{1.5 * TILE_SIZE + adj, 1.  * TILE_SIZE}, */
	/* 	{1.5 * TILE_SIZE - adj, 1.  * TILE_SIZE}, */
	/* 	{1.  * TILE_SIZE,       1.5 * TILE_SIZE - adj}, */
	/* 	{1.  * TILE_SIZE,       1.5 * TILE_SIZE + adj}, */
	/* 	{1.5 * TILE_SIZE - adj, 2.  * TILE_SIZE}, */
	/* 	{1.5 * TILE_SIZE + adj, 2.  * TILE_SIZE}, */
	/* 	{2.  * TILE_SIZE,       1.5 * TILE_SIZE + adj}, */
	/* }; */
	/* for (i = 0, context.me.angle = M_PI / 8; context.me.angle < 2 * M_PI; context.me.angle += M_PI_4, i++) { */
	/* 	check_point(&context, &ctrl_c[i], floor_dist); */
	/* } */

	/* map[0][0] = EMPTY; */
	/* map[0][2] = EMPTY; */
	/* map[2][0] = EMPTY; */
	/* map[2][2] = EMPTY; */
	/* for (i = 0, context.me.angle = M_PI_4; context.me.angle < 2 * M_PI; context.me.angle += M_PI_2, i++) { */
	/* 	check_point(&context, &ctrl_b[i], TILE_SIZE / 2 * M_SQRT2); */
	/* } */

	ft_arrdel(&context.map);
}
