/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/18 17:01:18 by mc                #+#    #+#             */
/*   Updated: 2017/04/20 13:15:23 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sdl.h"

void draw_line(t_uint *screen_pixels, t_point *a, t_point *b, t_uint color)
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

void draw_rect(t_uint *screen_pixels, SDL_Rect *rect, t_uint color)
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
