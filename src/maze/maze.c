/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maze.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 18:03:06 by mc                #+#    #+#             */
/*   Updated: 2017/04/09 13:50:01 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "maze.h"

#ifdef DEBUG_MODE
static void debug_map(t_arr *map)
{
	char **map_ptr;

	map_ptr = (char **)map->ptr;
	while (map_ptr - (char **)map->ptr < (long)map->length)
		ft_debugstr("map", *map_ptr++);
	ft_putendl("");
}

static void debug_walls(t_arr *walls)
{
	t_point *walls_ptr;

	walls_ptr = (t_point *)walls->ptr;
	while (walls_ptr - (t_point *)walls->ptr < (long)walls->length)
	{
		ft_debugnbr("wall.x", walls_ptr->x);
		ft_debugnbr("wall.y", walls_ptr->y);
		walls_ptr++;
	}
	ft_putendl("");
}
#endif //DEBUG_MODE



static void free_string(void *s, size_t n)
{
	(void)n;
	ft_memdel(s);
}

static void *cpy_wall(void *dest, const void *src, size_t n)
{
	return ft_memcpy(dest, *(t_point **)src, n);
}


static t_arr *create_empty_map(t_uint size)
{
	t_arr *map;
	t_uint i;

	map = ft_arrnew(size, sizeof(char *));
	map->del = free_string;
	i = size;
	while (i--)
		ft_arrpush(map, ft_memset(ft_memalloc(size + 1), WALL, size), -1);
	return (map);
}

static t_bool in_map(t_arr *map, t_point *p)
{
	return (p->x > 0 && p->y > 0 \
			&& p->x < map->length - 1 && p->y < map->length - 1);
}

static t_bool touch_one_empty_tile(t_arr *map, t_point *p)
{
	t_uint n_empty_tiles;

	if (!in_map(map, p) || MAP_CHAR(map->ptr, p->x, p->y) != WALL)
		return (FALSE);

	n_empty_tiles = 0;

	p->y -= 1; // top
	if (in_map(map, p) && MAP_CHAR(map->ptr, p->x, p->y) == EMPTY)
		n_empty_tiles++;

	p->y += 2; // bottom
	if (in_map(map, p) && MAP_CHAR(map->ptr, p->x, p->y) == EMPTY)
		n_empty_tiles++;

	p->y -= 1; // wait for it...
	p->x -= 1; // left
	if (in_map(map, p) && MAP_CHAR(map->ptr, p->x, p->y) == EMPTY)
		n_empty_tiles++;

	p->x += 2; // right
	if (in_map(map, p) && MAP_CHAR(map->ptr, p->x, p->y) == EMPTY)
		n_empty_tiles++;

	p->x -= 1; // restore

	return (n_empty_tiles == 1);
}

static void add_near_walls(t_arr *map, t_arr *walls, t_point p)
{
	MAP_CHAR(map->ptr, p.x, p.y) = EMPTY;

	p.y -= 1; // top
	if (touch_one_empty_tile(map, &p))
		ft_arrpush(walls, (void *)&p, -1);

	p.y += 2; // bottom
	if (touch_one_empty_tile(map, &p))
		ft_arrpush(walls, (void *)&p, -1);

	p.y -= 1; // wait for it...
	p.x -= 1; // left
	if (touch_one_empty_tile(map, &p))
		ft_arrpush(walls, (void *)&p, -1);

	p.x += 2; // right
	if (touch_one_empty_tile(map, &p))
		ft_arrpush(walls, (void *)&p, -1);

	/* p.x -= 1; // restore */
}

static void create_path(t_context *context, t_point *start, t_uint delay)
{
	t_arr *walls;
	t_point *p;
	t_point exit;

	walls = ft_arrnew(0, sizeof(t_point));
	walls->cpy = cpy_wall;
	add_near_walls(context->map, walls, *start);
	while (walls->length)
	{
#ifdef DEBUG_MODE
		/* debug_walls(walls);				/\* DEBUG *\/ */
#endif //DEBUG_MODE
		p = (t_point *)walls->ptr + ((t_uint)rand() % walls->length);
		if (touch_one_empty_tile(context->map, p))
		{
			exit = *p;
			add_near_walls(context->map, walls, *p);
			MAP_CHAR(context->map->ptr, exit.x, exit.y) = EXIT; //just to draw it another color...
			draw(context, FALSE);
			MAP_CHAR(context->map->ptr, exit.x, exit.y) = EMPTY;
			SDL_Delay(delay);
#ifdef DEBUG_MODE
			/* debug_map(context->map);				/\* DEBUG *\/ */
#endif //DEBUG_MODE
		}
		ft_arrpop(walls, (int)(p - (t_point *)walls->ptr));
	}
	if (context->map->length > 3) //no room for exit :/
		MAP_CHAR(context->map->ptr, exit.x, exit.y) = EXIT;
	ft_arrdel(&walls);
}

/**
** generate a randomized maze, based on this:
** en.wikipedia.org/wiki/Maze_generation_algorithm#Randomized_Prim.27s_algorithm
** @param: the map will be a SIZE * SIZE square, stored in CONTEXT->map
** CONTEXT used for map, player and renderer infos
*/
t_bool generate_maze(t_uint size, t_context *context) //TODO: multiply all coord by TILE_SIZE
{
	if (size < 3 //we need a square with walls around...
		|| size > 200) //TODO: check the drawing limits
		return (FALSE);

	srand((t_uint)time(NULL)); //move to init()?

	context->me.coord.x = 1;
	context->me.coord.y = 1;
	context->me.angle = 0;;

	context->map = create_empty_map(size);
	create_path(context, &context->me.coord, (t_uint)((MAZE_LATENCY - size) / pow(size, 2)));

	context->me.coord.x = TILE_SIZE + TILE_SIZE / 5;
	context->me.coord.y = TILE_SIZE + TILE_SIZE / 5;

#ifdef DEBUG_MODE
	if (context->renderer)
		debug_map(context->map);				/* DEBUG */
#endif //DEBUG_MODE

	draw(context, TRUE);
	while (context->renderer && !context->me.action)
	{
		handle_events(context);
		SDL_Delay(50);
	}

	return (TRUE);
}
