/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maze_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/18 18:11:09 by mc                #+#    #+#             */
/*   Updated: 2017/04/20 20:59:58 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** a few helpers functions for maze generation
*/

#include "maze.h"

/**
** this is used to free a map line from t_arr *MAP
*/
void free_string(void *s, size_t n)
{
	(void)n;
	ft_memdel(s);
}

/**
** this is used to copy a t_point WALL line from t_arr *WALLS
*/
void *cpy_wall(void *dest, const void *src, size_t n)
{
	return ft_memcpy(dest, *(t_point **)src, n);
}

static t_bool point_in_map(t_arr *map, t_point *p)
{
	return (p->x > 0 && p->y > 0 \
			&& p->x < map->length - 1 && p->y < map->length - 1);
}

t_bool touch_one_empty_tile(t_arr *map, t_point *p)
{
	t_uint n_empty_tiles;

	if (!point_in_map(map, p) || MAZE_CHAR(map->ptr, p->x, p->y) != WALL)
		return (FALSE);

	n_empty_tiles = 0;

	p->y -= 1; // top
	if (point_in_map(map, p) && MAZE_CHAR(map->ptr, p->x, p->y) == EMPTY)
		n_empty_tiles++;

	p->y += 2; // bottom
	if (point_in_map(map, p) && MAZE_CHAR(map->ptr, p->x, p->y) == EMPTY)
		n_empty_tiles++;

	p->y -= 1; // wait for it...
	p->x -= 1; // left
	if (point_in_map(map, p) && MAZE_CHAR(map->ptr, p->x, p->y) == EMPTY)
		n_empty_tiles++;

	p->x += 2; // right
	if (point_in_map(map, p) && MAZE_CHAR(map->ptr, p->x, p->y) == EMPTY)
		n_empty_tiles++;

	p->x -= 1; // restore

	return (n_empty_tiles == 1);
}
