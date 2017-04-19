/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_handlers-roll.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/18 18:04:37 by mc                #+#    #+#             */
/*   Updated: 2017/04/18 18:04:53 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sdl.h"

t_bool handler_roll(t_context *context, int key, t_uint type)
{
	if (key != KEYA_ROLL && key != KEYB_ROLL)
		return (FALSE);

	if (type == SDL_KEYDOWN)
		context->me.action |= A_ROLL;
	else
		context->me.action &= (t_uint)~A_ROLL;

	return (TRUE);
}

t_bool handler_unroll(t_context *context, int key, t_uint type)
{
	if (key != KEYA_UNROLL && key != KEYB_UNROLL)
		return (FALSE);

	if (type == SDL_KEYDOWN)
		context->me.action |= A_UNROLL;
	else
		context->me.action &= (t_uint)~A_UNROLL;

	return (TRUE);
}
