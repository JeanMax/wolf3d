/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 00:30:53 by mc                #+#    #+#             */
/*   Updated: 2017/03/26 04:01:12 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "wolf3d.h"

static t_bool finit(t_context *context)
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

static t_bool init(t_context *context)
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
												 SDL_RENDERER_ACCELERATED)))
		return (finit(context) && FALSE);

	SDL_SetRenderDrawColor(context->renderer, BLACK);
	SDL_RenderClear(context->renderer);

	return (TRUE);
}

static void game_loop(t_context *context)
{
	t_uint tick;
	t_uint tack;
	t_uint maze_size;

	maze_size = INITIAL_MAZE_SIZE;
	while (generate_maze(maze_size, context))
	{
		tack = 0;
		while (tack < 120 * 1e3) //TODO
		{
			tick = SDL_GetTicks();

			if (tick - tack > MSPF)
			{
				/* ft_debugnbr("FPS", tick - tack); /\* DEBUG *\/ */
				handle_events(context);
				update_player(&context->me);
				raycaster(context);
				SDL_RenderPresent(context->renderer);
				tack = tick;
			}
			SDL_Delay(10);
		}
		maze_size *= 2;
	}
}

t_bool kthxbye(t_context *context)
{
	finit(context);
	exit(EXIT_SUCCESS);

	return (FALSE);
}

int		main(void)
{
	t_context context;

	if (!init(&context))
		return (EXIT_FAILURE);


	game_loop(&context);

	/* DEBUG */
	/* context.me.angle = 0; */
	/* while (context.me.angle < 2 * M_PI) */
	/* { */
	/* 	raycaster(&context); */
	/* 	handle_events(&context); */
	/* 	SDL_Delay(50); */
	/* 	context.me.angle += M_PI / 50; */
	/* } */
	/* DEBUG */

	finit(&context);

	return (EXIT_SUCCESS);
}
