/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/09 13:18:26 by mc                #+#    #+#             */
/*   Updated: 2017/04/12 11:39:59 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "wolf3d.h"

//TODO: same macros as raycaster.c, move them to some header

#define MAP_CHAR(MAP, X, Y) (*(*((char **)(MAP) + (int)((Y) / TILE_SIZE)) + (int)((X) / TILE_SIZE)))

t_bool finit(t_context *context)
{
	if (context->renderer)
		SDL_DestroyRenderer(context->renderer);
	if (context->window)
		SDL_DestroyWindow(context->window);
	SDL_Quit();
	if (context->map)
		ft_arrdel(&context->map);

	return (TRUE);
}

t_bool init(t_context *context)
{
	ft_bzero(context, sizeof(t_context));

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return (FALSE);

	if (!(context->window = SDL_CreateWindow("zboub",
									SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED,
									PROJ_WIDTH, PROJ_HEIGHT,
									SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL)))
		return (finit(context) && FALSE);

	if (!(context->renderer = SDL_CreateRenderer(context->window,
												 -1,
												 SDL_RENDERER_PRESENTVSYNC)))
		return (finit(context) && FALSE);

	SDL_SetRenderDrawColor(context->renderer, SDL_BLACK);
	SDL_RenderClear(context->renderer);

	return (TRUE);
}

/**
** HAI! l3t's fr33 3vryth1ng and 3x1t!
** @param: CONTEXT used for map and renderer infos
*/
t_bool kthxbye(t_context *context)
{
	finit(context);
	exit(EXIT_SUCCESS);

	return (FALSE);
}

/**
** where shits get real
** draw stuff every frame, if an action has been registred
** @param: CONTEXT used for map, player and renderer infos
*/
void game_loop(t_context *context, t_uint maze_size)
{
	while (generate_maze(maze_size, context))
	{
		context->me.status |= S_LIVE;
		draw(context, TRUE);
		while (MAP_CHAR(context->map->ptr, context->me.coord.x, context->me.coord.y) != EXIT)
		{
			draw(context, FALSE);
			SDL_Delay(10);
		}
		context->me.status &= (t_uint)~S_LIVE;
		maze_size *= 2;
	}
	ft_putstr(CLR_GREEN"You won!\n"CLR_RESET);
}
