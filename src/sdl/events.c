/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:49:42 by mc                #+#    #+#             */
/*   Updated: 2017/09/12 17:13:37 by mcanal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sdlux.h"

static t_bool	handler_quit(t_context *context, int key, t_uint type)
{
	if (key != SDLK_ESCAPE)
		return (FALSE);
	(void)type;
	kthxbye(context);
	return (TRUE);
}

/*
** handle SDL events
** for keyboard events, call the appropriate subfunction
** for window closing, exit; for window moving, redraw
** @param: CONTEXT used for map, player and renderer infos
*/

void			handle_events(t_context *context)
{
	SDL_Event		event;
	t_uint			i;
	static t_bool	(*handler[])(t_context *, int, t_uint) = {
		handler_quit, handler_move_up, handler_move_down, handler_move_left,
		handler_move_right, handler_roll, handler_unroll, handler_run,
		handler_map, NULL
	};

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			kthxbye(context);
		else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
		{
			i = 0;
			while (handler[i] \
					&& !handler[i](context, event.key.keysym.sym, event.type))
				i++;
		}
		else if (event.type == SDL_WINDOWEVENT \
				&& (event.window.event == SDL_WINDOWEVENT_EXPOSED \
					|| event.window.event == SDL_WINDOWEVENT_RESIZED))
			draw(context, TRUE);
	}
}
