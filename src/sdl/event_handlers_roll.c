/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_handlers_roll.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcanal <mc.maxcanal@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/12 17:14:50 by mcanal            #+#    #+#             */
/*   Updated: 2017/09/12 17:17:18 by mcanal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** various events handlers used in events.c, focused on player rotation
*/

#include "sdlux.h"

t_bool	handler_roll(t_context *context, int key, t_uint type)
{
	if (key != KEYA_ROLL && key != KEYB_ROLL)
		return (FALSE);
	if (type == SDL_KEYDOWN)
		context->me.action |= A_ROLL;
	else
		context->me.action &= (t_uint)~A_ROLL;
	return (TRUE);
}

t_bool	handler_unroll(t_context *context, int key, t_uint type)
{
	if (key != KEYA_UNROLL && key != KEYB_UNROLL)
		return (FALSE);
	if (type == SDL_KEYDOWN)
		context->me.action |= A_UNROLL;
	else
		context->me.action &= (t_uint)~A_UNROLL;
	return (TRUE);
}
