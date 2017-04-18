/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/25 00:48:51 by mc                #+#    #+#             */
/*   Updated: 2017/04/18 15:46:47 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "maze.h"

#define DOUBLE_PRECISION (1e-6)
#define ZERO(X) ((X) > -DOUBLE_PRECISION && (X) < DOUBLE_PRECISION)

static void draw_line(int *screen_pixels, t_point *a, t_point *b, int color)
{
	double x;
	double y;
	double inc_x;
	double inc_y;

	a->x = (double)(int)a->x;
	a->y = (double)(int)a->y;
	b->x = (double)(int)b->x;
	b->y = (double)(int)b->y;
	x = a->x;
	y = a->y;
	inc_x = (b->x - a->x) / MAX(ABS(b->x - a->x), ABS(b->y - a->y));
	inc_y = (b->y - a->y) / MAX(ABS(b->x - a->x), ABS(b->y - a->y));
	while (TRUE)
	{
		screen_pixels[(int)(round(x)) + (int)(round(y)) * PROJ_WIDTH] = color;
		if ((int)(round(x)) == (int)b->x && (int)(round(y)) == (int)b->y)
			break ;
		x += inc_x;
		y += inc_y;
	}
}

static void draw_rect(int *screen_pixels, SDL_Rect *rect, int color)
{
	int x;
	int y;

	y = rect->y;
	while (y - rect->y < rect->h)
	{
		x = rect->x;
		while (x - rect->x < rect->w)
		{
			screen_pixels[x + y * PROJ_WIDTH] = color;
			x++;
		}
		y++;
	}
}


static void to_map_coord(t_point *dst, t_point *p, double map_size)
{
	t_point offset;
	double inc;

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

static void draw_me_on_map(t_context *context)
{
	int x;
	double angle;
	double wall_dist;			/* DEBUG */
	t_point wall_coord;
	t_point me;

	me.x = context->me.coord.x / TILE_SIZE;
	me.y = context->me.coord.y / TILE_SIZE;
	to_map_coord(&me, &me, (double)context->map->length);

	SDL_SetRenderDrawColor(context->renderer, SDL_RED);
	angle = context->me.angle - FOV / 2;
	x = 0;
	/* DEBUG(CLR_WHITE"\nme: x:%f, y:%f, angle:%f ", context->me.coord.x / TILE_SIZE, context->me.coord.y / TILE_SIZE, context->me.angle);	/\* DEBUG *\/ */
	while (x < PROJ_WIDTH)
	{
		if (angle < 0)
			angle += 2 * M_PI;
		if (angle >= 2 * M_PI)
			angle -= 2 * M_PI;

		if ((wall_dist = get_wall_coord(&wall_coord, context, angle)) > 0)
		{
			wall_coord.x /= TILE_SIZE;
			wall_coord.y /= TILE_SIZE;

#ifdef DEBUG_MODE
			if (ZERO(angle - context->me.angle))
			{
				SDL_SetRenderDrawColor(context->renderer, SDL_BLACK); /* DEBUG */
				/* DEBUG("wall: x:%f, y:%f, angle/pi:%f\n", wall_coord.x, wall_coord.y, angle / (2 * M_PI)); /\* DEBUG *\/ */
			}
#endif //DEBUG_MODE


			to_map_coord(&wall_coord, &wall_coord, (double)context->map->length);
			draw_line(context->screen_pixels, &me, &wall_coord, 0xff000000);
		}
		/* else */
			/* DEBUG(CLR_RED"buggy-wall: x:%f, y:%f, angle:%f", wall_coord.x, wall_coord.y, angle); /\* DEBUG *\/ */

		angle += ANGLE_PER_RAY;
		x++;
	}
}
//TODO: hook into raycaster with another renderer for map (avoid casting PROJ_WIDTH rays again)

/**
** draw the world in 2d, as you birdman see it (no?)
** also draw the player field of view, because it's cool
** @param: CONTEXT used for map, player and renderer infos
*/
void draw_map(t_context *context) //TODO: delete x,y
{
	SDL_Rect rect;
	t_point map_index;
	t_point tmp;

	rect.w = (int)(MIN(PROJ_WIDTH, PROJ_HEIGHT) / (double)context->map->length + 1);
	rect.h = rect.w; //HAI! 1'm a square!1!
	map_index.y = 0;
	while (map_index.y < context->map->length)
	{
		map_index.x = 0;
		while (map_index.x < context->map->length)
		{
			if (MAP_CHAR(context->map->ptr, map_index.x, map_index.y) != WALL)
			{
				to_map_coord(&tmp, &map_index, (double)context->map->length);
				rect.x = (int)tmp.x;
				rect.y = (int)tmp.y;
				if (MAP_CHAR(context->map->ptr, map_index.x, map_index.y) == EXIT)
					draw_rect(context->screen_pixels, &rect, 0x00ff0000);
				else
					draw_rect(context->screen_pixels, &rect, 0xffffff00);
			}
			map_index.x++;
		}
		map_index.y++;
	}
	/* DEBUG("%dx%d %d %f\n", rect.x, rect.y, rect.w, inc); /\* DEBUG *\/ */
	if (context->me.status & S_LIVE)
		draw_me_on_map(context);
}

/**
** call the appropriate draw function based on CONTEXT->me.status
** @param: CONTEXT used for map, player and renderer infos
*/
void draw(t_context *context, t_bool force)
{
	static t_uint tack = 0;
	t_uint tick;

	tick = SDL_GetTicks();
	if (!context->renderer || (tick - tack < MSPF && !force))
		return;

#ifdef DEBUG_MODE
	if (tick - tack > 5 * MSPF / 4)
		DEBUG(CLR_RED"lag: %dms (MSPF: %dms/frame)", tick - tack, MSPF);
#endif
	tack = tick;

	handle_events(context);
	update_player(context);
	if (!context->me.action && !force \
		)
		/* && !(context->me.status & S_LIVE)) */
		return ;

	SDL_SetRenderDrawColor(context->renderer, SDL_BLACK);
	SDL_RenderClear(context->renderer);

	if (context->me.status & S_LIVE)
	{
		raycaster(context);

		if (context->me.status & S_MAP)
			draw_map(context);
	}
	else
		draw_map(context);

	SDL_UpdateTexture(context->screen_texture, NULL, \
					  context->screen_pixels, PROJ_WIDTH * sizeof(int));
	SDL_RenderCopy(context->renderer, context->screen_texture,	\
				   NULL, NULL);
	SDL_RenderPresent(context->renderer);
}
//TODO:move that
