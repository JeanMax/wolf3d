/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/25 00:48:51 by mc                #+#    #+#             */
/*   Updated: 2017/03/28 17:52:21 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "maze.h"

static void draw_me_on_map(t_context *context)
{
	int x;
	double angle;
	double wall_dist;			/* DEBUG */
	t_point wall_coord;

	SDL_SetRenderDrawColor(context->renderer, RED);
	angle = context->me.angle - FOV / 2;
	x = 0;
	while (x < PROJ_WIDTH)
	{
		if (angle < 0)
			angle += 2 * M_PI;
		if (angle >= 2 * M_PI)
			angle -= 2 * M_PI;

		if ((wall_dist = get_wall_coord(&wall_coord, context, angle)) > 0)
			SDL_RenderDrawLine(context->renderer, \
							   (int)context->me.coord.x, (int)context->me.coord.y, \
							   (int)wall_coord.x, (int)wall_coord.y);

		angle += ANGLE_PER_RAY;
		x++;
	}
}

/**
** draw the world in 2d, as you birdman see it (no?)
** also draw the player field of view, because it's cool
** @param: CONTEXT used for map, player and renderer infos
*/
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

/**
** call the appropriate draw function based on CONTEXT->me.status
** @param: CONTEXT used for map, player and renderer infos
*/
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
