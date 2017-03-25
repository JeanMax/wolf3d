/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:49:42 by mc                #+#    #+#             */
/*   Updated: 2017/03/25 19:35:35 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "sdl.h"

static void handle_key_down(int key)
{
	if (key == SDLK_ESCAPE)
		exit(EXIT_SUCCESS); //TODO: close window, quit sdl, blabla
}

void handle_events(SDL_Renderer *renderer)
{
	SDL_Event	event;
	t_uint tick;
	static t_uint tack = 0;

	tick = SDL_GetTicks();
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			exit(EXIT_SUCCESS); //TODO: close window, quit sdl, blabla
		else if (event.type == SDL_KEYDOWN)
			handle_key_down(event.key.keysym.sym);
	}

	if (tick - tack > MSPF) //TODO: move
	{
		/* ft_debugnbr("zboub", tick - tack); /\* DEBUG *\/ */
		SDL_RenderPresent(renderer);
		tack = tick;
	}
}
