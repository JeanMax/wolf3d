/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/25 00:48:51 by mc                #+#    #+#             */
/*   Updated: 2017/03/25 03:19:08 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "maze.h"

void draw_map(t_arr *map, t_point *start, t_point *exit, SDL_Renderer *renderer)
{
	SDL_Rect rect;
	t_uint y;
	t_uint x;

	/* SDL_SetRenderDrawColor(renderer, BLACK); */
	/* SDL_RenderClear(renderer); */
	ft_bzero(&rect, sizeof(SDL_Rect));

	rect.w = PROJ_WIDTH / map->length;
	rect.h = PROJ_HEIGHT / map->length;
	y = 0;
	while (y < map->length)
	{
		x = 0;
		rect.x = 0;
		while (x < map->length)
		{
			if (x == (t_uint)start->x && y == (t_uint)start->y)
				SDL_SetRenderDrawColor(renderer, GREEN);
			else if (x == (t_uint)exit->x && y == (t_uint)exit->y)
				SDL_SetRenderDrawColor(renderer, RED);
			else
				SDL_SetRenderDrawColor(renderer, WHITE);

			if (MAP_CHAR(map->ptr, x, y) != WALL)
				SDL_RenderFillRect(renderer, &rect);
			rect.x += rect.w;
			x++;
		}
		rect.y += rect.h;
		y++;
	}

	handle_events(renderer);
	/* SDL_Delay(10); */
}
