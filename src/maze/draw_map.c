/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/25 00:48:51 by mc                #+#    #+#             */
/*   Updated: 2017/09/12 17:25:20 by mcanal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maze.h"

static void	to_map_coord(t_point *dst, t_point *p, double map_size)
{
	t_point offset;
	double	inc;

	if (PROJ_HEIGHT > PROJ_WIDTH)
	{
		offset.x = 0;
		offset.y = (PROJ_HEIGHT - PROJ_WIDTH) / 2;
		inc = PROJ_WIDTH / map_size;
	}
	else
	{
		offset.x = (PROJ_WIDTH - PROJ_HEIGHT) / 2;
		offset.y = 0;
		inc = PROJ_HEIGHT / map_size;
	}
	dst->x = p->x * inc + offset.x;
	dst->y = p->y * inc + offset.y;
}

static void	draw_me_on_map(t_context *context)
{
	int				x;
	t_polar_point	wall;
	t_point			me;

	me.x = context->me.coord.x / TILE_SIZE;
	me.y = context->me.coord.y / TILE_SIZE;
	to_map_coord(&me, &me, (double)context->map->length);
	wall.angle = mod2pi(context->me.angle - FOV / 2);
	x = PROJ_WIDTH - 1;
	while (x >= 0)
	{
		if (get_wall(context, &wall))
		{
			wall.coord.x /= TILE_SIZE;
			wall.coord.y /= TILE_SIZE;
			to_map_coord(&wall.coord,
						&wall.coord,
						(double)context->map->length);
			draw_line(context->screen_pixels, &me, &wall.coord, 0xff000000);
		}
		wall.angle = mod2pi(wall.angle + ANGLE_PER_RAY);
		x--;
	}
}

static void	draw_map_loop(SDL_Rect *rect, t_uint *screen_pixels, t_arr *map)
{
	t_point map_index;
	t_point tmp;

	map_index.y = 0;
	while (map_index.y < map->length)
	{
		map_index.x = 0;
		while (map_index.x < map->length)
		{
			if (MAZE_CHAR(map->ptr, map_index.x, map_index.y) != WALL)
			{
				to_map_coord(&tmp, &map_index, (double)map->length);
				rect->x = (int)tmp.x;
				rect->y = (int)tmp.y;
				if (MAZE_CHAR(map->ptr, map_index.x, map_index.y) == EXIT)
					draw_rect(screen_pixels, rect, 0x00ff0000);
				else
					draw_rect(screen_pixels, rect, 0xffffff00);
			}
			map_index.x++;
		}
		map_index.y++;
	}
}

/*
** draw the world in 2d, as you bird-person see it (no?)
** also draw the player field of view, because it's cool
** @param: CONTEXT used for map, player and renderer infos
*/

void		draw_map(t_context *context)
{
	SDL_Rect rect;

	rect.w = (int)(MIN(PROJ_WIDTH, PROJ_HEIGHT) \
					/ (double)context->map->length + 1);
	rect.h = rect.w;
	draw_map_loop(&rect, context->screen_pixels, context->map);
	if (context->me.status & S_LIVE)
		draw_me_on_map(context);
}
