/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maze_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcanal <mc.maxcanal@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/12 17:22:24 by mcanal            #+#    #+#             */
/*   Updated: 2017/09/12 17:23:43 by mcanal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** a few helpers functions for maze generation
*/

#include "maze.h"

/*
** this is used to free a map line from t_arr *MAP
*/

void			free_string(void *s, size_t n)
{
	(void)n;
	ft_memdel(s);
}

/*
** this is used to copy a t_point WALL line from t_arr *WALLS
*/

void			*cpy_wall(void *dest, const void *src, size_t n)
{
	return (ft_memcpy(dest, *(t_point **)src, n));
}

static t_bool	point_in_map(t_arr *map, t_point *p)
{
	return (p->x > 0 && p->y > 0 \
			&& p->x < map->length - 1 && p->y < map->length - 1);
}

t_bool			touch_one_empty_tile(t_arr *map, t_point *p)
{
	t_uint n_empty_tiles;

	if (!point_in_map(map, p) || MAZE_CHAR(map->ptr, p->x, p->y) != WALL)
		return (FALSE);
	n_empty_tiles = 0;
	p->y -= 1;
	if (point_in_map(map, p) && MAZE_CHAR(map->ptr, p->x, p->y) == EMPTY)
		n_empty_tiles++;
	p->y += 2;
	if (point_in_map(map, p) && MAZE_CHAR(map->ptr, p->x, p->y) == EMPTY)
		n_empty_tiles++;
	p->y -= 1;
	p->x -= 1;
	if (point_in_map(map, p) && MAZE_CHAR(map->ptr, p->x, p->y) == EMPTY)
		n_empty_tiles++;
	p->x += 2;
	if (point_in_map(map, p) && MAZE_CHAR(map->ptr, p->x, p->y) == EMPTY)
		n_empty_tiles++;
	p->x -= 1;
	return (n_empty_tiles == 1);
}
