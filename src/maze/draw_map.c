/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/25 00:48:51 by mc                #+#    #+#             */
/*   Updated: 2017/03/27 21:20:58 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "maze.h"

static void draw_me_on_map(t_context *context)
{
	SDL_SetRenderDrawColor(context->renderer, RED);
	SDL_RenderDrawLine(context->renderer, context->me.coord.x - 10, context->me.coord.y - 10, context->me.coord.x + 10, context->me.coord.y + 10);
	SDL_RenderDrawLine(context->renderer, context->me.coord.x + 10, context->me.coord.y - 10, context->me.coord.x - 10, context->me.coord.y + 10);
}

void draw_map(t_context *context) //TODO: delete x,y
{
	SDL_Rect rect;
	t_uint y;
	t_uint x;

	rect.w = PROJ_WIDTH / context->map->length;
	rect.h = PROJ_HEIGHT / context->map->length;
	y = 0;
	rect.y = 0;
	while (y < context->map->length)
	{
		x = 0;
		rect.x = 0;
		while (x < context->map->length)
		{
			if (MAP_CHAR(context->map->ptr, x, y) == EXIT)
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

	if (context->me.status & S_LIVE)
		draw_me_on_map(context);
}

void draw(t_context *context)
{
	SDL_SetRenderDrawColor(context->renderer, BLACK);
	SDL_RenderClear(context->renderer);

	if (context->me.status & S_LIVE)
	{
		raycaster(context);
		if (context->me.status & S_MAP)
			draw_map(context);
	}
	else
		draw_map(context);

	SDL_RenderPresent(context->renderer);
}
