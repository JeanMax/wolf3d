/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_handlers-move.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/18 18:02:54 by mc                #+#    #+#             */
/*   Updated: 2017/04/20 21:02:31 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** various events handlers used in events.c, focused on player movement
*/

#include "sdl.h"

t_bool handler_move_up(t_context *context, int key, t_uint type)
{
	if (key != KEYA_UP && key != KEYB_UP)
		return (FALSE);

	if (type == SDL_KEYDOWN)
		context->me.action |= A_UP;
	else
		context->me.action &= (t_uint)~A_UP;

	return (TRUE);
}

t_bool handler_move_down(t_context *context, int key, t_uint type)
{
	if (key != KEYA_DOWN && key != KEYB_DOWN)
		return (FALSE);

	if (type == SDL_KEYDOWN)
		context->me.action |= A_DOWN;
	else
		context->me.action &= (t_uint)~A_DOWN;

	return (TRUE);
}

t_bool handler_move_left(t_context *context, int key, t_uint type)
{
	if (key != KEYA_LEFT && key != KEYB_LEFT)
		return (FALSE);

	if (type == SDL_KEYDOWN)
		context->me.action |= A_LEFT;
	else
		context->me.action &= (t_uint)~A_LEFT;

	return (TRUE);
}

t_bool handler_move_right(t_context *context, int key, t_uint type)
{
	if (key != KEYA_RIGHT && key != KEYB_RIGHT)
		return (FALSE);

	if (type == SDL_KEYDOWN)
		context->me.action |= A_RIGHT;
	else
		context->me.action &= (t_uint)~A_RIGHT;

	return (TRUE);
}
