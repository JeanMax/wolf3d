/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_generate_maze.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/09 13:55:48 by mc                #+#    #+#             */
/*   Updated: 2017/04/11 16:10:08 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d_test.h"

void test_generate_maze()
{
	t_context context;

	bzero(&context, sizeof(t_context));
	MU_ASSERT_FATAL(!context.window && !context.renderer, \
					"man, you don't want sdl doing stuffs right now");
	MU_ASSERT(generate_maze(INITIAL_MAZE_SIZE, &context),	\
			  "nop... size:%d", INITIAL_MAZE_SIZE);
	MU_ASSERT(context.map, \
			  "this was supposed to generate a map...");
	ft_arrdel(&context.map);

	bzero(&context, sizeof(t_context));
	MU_ASSERT(generate_maze(3, &context),	\
			  "nop... size:%d", 3);
	MU_ASSERT(context.map, \
			  "this was supposed to generate a map...");
	MU_ASSERT(context.map->length == 3, \
			  "nop... length:%d", (int)context.map->length);
	char **map = context.map->ptr;
	for (int i = 0; i < 3; i++) {
		MU_ASSERT(map[0][i] == WALL, \
				  "test (%c) should be a wall (i:%d)", map[0][i], i);
		MU_ASSERT(map[2][i] == WALL, \
				  "test (%c) should be a wall (i:%d)", map[2][i], i);
	}
	MU_ASSERT(map[1][0] == WALL,							\
			  "test (%c) should be a wall", map[1][0]);
	MU_ASSERT(map[1][2] == WALL,							\
			  "test (%c) should be a wall", map[1][2]);
	MU_ASSERT(map[1][1] != WALL,							\
			  "test (%c) should be empty", map[1][1]);

	ft_arrdel(&context.map);
}
