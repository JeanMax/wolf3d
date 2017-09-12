/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maze.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 18:03:06 by mc                #+#    #+#             */
/*   Updated: 2017/09/12 17:20:25 by mcanal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maze.h"

static t_arr	*create_empty_map(t_uint size)
{
	t_arr	*map;
	t_uint	i;

	map = ft_arrnew(size, sizeof(char *));
	map->del = free_string;
	i = size;
	while (i--)
		ft_arrpush(map, ft_memset(ft_memalloc(size + 1), WALL, size), -1);
	return (map);
}

static void		add_near_walls(t_arr *map, t_arr *walls, t_point p)
{
	MAZE_CHAR(map->ptr, p.x, p.y) = EMPTY;
	p.y -= 1;
	if (touch_one_empty_tile(map, &p))
		ft_arrpush(walls, (void *)&p, -1);
	p.y += 2;
	if (touch_one_empty_tile(map, &p))
		ft_arrpush(walls, (void *)&p, -1);
	p.y -= 1;
	p.x -= 1;
	if (touch_one_empty_tile(map, &p))
		ft_arrpush(walls, (void *)&p, -1);
	p.x += 2;
	if (touch_one_empty_tile(map, &p))
		ft_arrpush(walls, (void *)&p, -1);
}

static void		create_path(t_context *context, t_point *start, t_uint delay)
{
	t_arr	*walls;
	t_point	*p;
	t_point	exit;

	walls = ft_arrnew(0, sizeof(t_point));
	walls->cpy = cpy_wall;
	add_near_walls(context->map, walls, *start);
	while (walls->length)
	{
		p = (t_point *)walls->ptr + ((t_uint)rand() % walls->length);
		if (touch_one_empty_tile(context->map, p))
		{
			exit = *p;
			add_near_walls(context->map, walls, *p);
			MAZE_CHAR(context->map->ptr, exit.x, exit.y) = EXIT;
			draw(context, TRUE);
			MAZE_CHAR(context->map->ptr, exit.x, exit.y) = EMPTY;
			SDL_Delay(delay);
		}
		ft_arrpop(walls, (int)(p - (t_point *)walls->ptr));
	}
	if (context->map->length == 3)
		ft_memcpy(&exit, &context->me.coord, sizeof(t_point));
	MAZE_CHAR(context->map->ptr, exit.x, exit.y) = EXIT;
	ft_arrdel(&walls);
}

/*
** generate a randomized maze, based on this:
** en.wikipedia.org/wiki/Maze_generation_algorithm#Randomized_Prim.27s_algorithm
** @param: the map will be a SIZE * SIZE square, stored in CONTEXT->map
** CONTEXT used for map, player and renderer infos
*/

t_bool			generate_maze(t_uint size, t_context *context)
{
	if (size < MIN_MAZE_SIZE || size > MAX_MAZE_SIZE)
		return (FALSE);
	srand((t_uint)time(NULL));
	context->me.coord.x = 1.;
	context->me.coord.y = context->me.coord.x;
	context->me.angle = 0;
	if (context->map)
		ft_arrdel(&context->map);
	context->map = create_empty_map(size);
	create_path(context,
				&context->me.coord,
				(t_uint)((MAZE_LATENCY - size) / pow(size, 2)));
	context->me.coord.x = TILE_SIZE + TILE_SIZE / 5;
	context->me.coord.y = context->me.coord.x;
	draw(context, TRUE);
	while (context->renderer && !context->me.action)
	{
		handle_events(context);
		SDL_Delay(50);
	}
	return (TRUE);
}
