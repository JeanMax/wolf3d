/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_handlers_status.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcanal <mc.maxcanal@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/12 17:15:21 by mcanal            #+#    #+#             */
/*   Updated: 2017/09/12 17:17:24 by mcanal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** various events handlers used in events.c, focused on player status update
*/

#include "sdlux.h"

t_bool	handler_run(t_context *context, int key, t_uint type)
{
	if (key != KEYA_RUN && key != KEYB_RUN)
		return (FALSE);
	if (type == SDL_KEYDOWN)
		context->me.status |= S_RUN;
	else
		context->me.status &= (t_uint)~S_RUN;
	return (TRUE);
}

t_bool	handler_map(t_context *context, int key, t_uint type)
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
