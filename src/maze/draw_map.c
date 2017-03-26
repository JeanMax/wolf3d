/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/25 00:48:51 by mc                #+#    #+#             */
/*   Updated: 2017/03/26 01:47:27 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "maze.h"

void draw_map(t_context *context, t_point *start, t_point *exit)
{
	SDL_Rect rect;
	t_uint y;
	t_uint x;

	ft_bzero(&rect, sizeof(SDL_Rect));

	rect.w = PROJ_WIDTH / context->map->length;
	rect.h = PROJ_HEIGHT / context->map->length;
	y = 0;
	while (y < context->map->length)
	{
		x = 0;
		rect.x = 0;
		while (x < context->map->length)
		{
			if (x == (t_uint)start->x && y == (t_uint)start->y)
				SDL_SetRenderDrawColor(context->renderer, GREEN);
			else if (x == (t_uint)exit->x && y == (t_uint)exit->y)
				SDL_SetRenderDrawColor(context->renderer, RED);
			else
				SDL_SetRenderDrawColor(context->renderer, WHITE);

			if (MAP_CHAR(context->map->ptr, x, y) != WALL)
				SDL_RenderFillRect(context->renderer, &rect);
			rect.x += rect.w;
			x++;
		}
		rect.y += rect.h;
		y++;
	}

	handle_events(context);
	/* SDL_Delay(10); */
}
