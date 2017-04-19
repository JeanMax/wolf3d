/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_handlers-status.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/18 18:06:17 by mc                #+#    #+#             */
/*   Updated: 2017/04/18 18:06:37 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sdl.h"

t_bool handler_run(t_context *context, int key, t_uint type)
{
	if (key != KEYA_RUN && key != KEYB_RUN)
		return (FALSE);

	if (type == SDL_KEYDOWN)
		context->me.status |= S_RUN;
	else
		context->me.status &= (t_uint)~S_RUN;

	return (TRUE);
}

t_bool handler_map(t_context *context, int key, t_uint type)
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
