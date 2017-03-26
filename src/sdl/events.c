/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:49:42 by mc                #+#    #+#             */
/*   Updated: 2017/03/26 03:02:40 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "sdl.h"

static void handle_key_down(t_context *context, int key)
{
	if (key == SDLK_ESCAPE)
		kthxbye(context);
}

void handle_events(t_context *context)
{
	SDL_Event	event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			kthxbye(context);
		else if (event.type == SDL_KEYDOWN)
			handle_key_down(context, event.key.keysym.sym);
	}
}
