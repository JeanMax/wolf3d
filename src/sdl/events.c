/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:49:42 by mc                #+#    #+#             */
/*   Updated: 2017/04/09 01:59:59 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "sdl.h"

static t_bool handler_quit(t_context *context, int key, t_uint type)
{
	if (key != SDLK_ESCAPE)
		return (FALSE);

	(void)type;
	kthxbye(context);

	return (TRUE);
}

static t_bool handler_move_up(t_context *context, int key, t_uint type)
{
	if (key != KEYA_UP && key != KEYB_UP)
		return (FALSE);

	if (type == SDL_KEYDOWN)
		context->me.action |= A_UP;
	else
		context->me.action &= (t_uint)~A_UP;

	return (TRUE);
}

static t_bool handler_move_down(t_context *context, int key, t_uint type)
{
	if (key != KEYA_DOWN && key != KEYB_DOWN)
		return (FALSE);

	if (type == SDL_KEYDOWN)
		context->me.action |= A_DOWN;
	else
		context->me.action &= (t_uint)~A_DOWN;

	return (TRUE);
}

static t_bool handler_move_left(t_context *context, int key, t_uint type)
{
	if (key != KEYA_LEFT && key != KEYB_LEFT)
		return (FALSE);

	if (type == SDL_KEYDOWN)
		context->me.action |= A_LEFT;
	else
		context->me.action &= (t_uint)~A_LEFT;

	return (TRUE);
}

static t_bool handler_move_right(t_context *context, int key, t_uint type)
{
	if (key != KEYA_RIGHT && key != KEYB_RIGHT)
		return (FALSE);

	if (type == SDL_KEYDOWN)
		context->me.action |= A_RIGHT;
	else
		context->me.action &= (t_uint)~A_RIGHT;

	return (TRUE);
}

static t_bool handler_roll(t_context *context, int key, t_uint type)
{
	if (key != KEYA_ROLL && key != KEYB_ROLL)
		return (FALSE);

	if (type == SDL_KEYDOWN)
		context->me.action |= A_ROLL;
	else
		context->me.action &= (t_uint)~A_ROLL;

	return (TRUE);
}

static t_bool handler_unroll(t_context *context, int key, t_uint type)
{
	if (key != KEYA_UNROLL && key != KEYB_UNROLL)
		return (FALSE);

	if (type == SDL_KEYDOWN)
		context->me.action |= A_UNROLL;
	else
		context->me.action &= (t_uint)~A_UNROLL;

	return (TRUE);
}

static t_bool handler_run(t_context *context, int key, t_uint type)
{
	if (key != KEYA_RUN && key != KEYB_RUN)
		return (FALSE);

	if (type == SDL_KEYDOWN)
		context->me.status |= S_RUN;
	else
		context->me.status &= (t_uint)~S_RUN;

	return (TRUE);
}

static t_bool handler_map(t_context *context, int key, t_uint type)
{
	if (key != KEYA_MAP && key != KEYB_MAP)
		return (FALSE);


	if (type == SDL_KEYDOWN)
	{
		if (!(context->me.status & S_MAP))
		{
			context->me.status |= S_MAP;
			draw(context, TRUE);
		}
		else
			context->me.status |= S_MAP;
	}
	else
	{
		context->me.status &= (t_uint)~S_MAP;
		draw(context, TRUE);
	}

	return (TRUE);
}

/**
** handle SDL events
** for keyboard events, call the appropriate subfunction
** for window closing, exit; for window moving, redraw
** @param: CONTEXT used for map, player and renderer infos
*/
void handle_events(t_context *context)
{
	SDL_Event	event;
	t_uint      i;
	static t_bool (*handler[])(t_context *, int, t_uint) = {
		handler_quit,
		handler_move_up,
		handler_move_down,
		handler_move_left,
		handler_move_right,
		handler_roll,
		handler_unroll,
		handler_run,
		handler_map,
		NULL
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
				 && event.window.event == SDL_WINDOWEVENT_EXPOSED)
			draw(context, TRUE);
	}
}
