/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 00:30:53 by mc                #+#    #+#             */
/*   Updated: 2017/03/25 02:34:29 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "wolf3d.h"

int		main(void)
{
    SDL_Window	*window;
	SDL_Renderer *renderer;
	t_arr       *map;
	t_player    player;

	ft_bzero(&player, sizeof(t_player));

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return (EXIT_FAILURE);

	if (!(window = SDL_CreateWindow("zboub",
									SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
									PROJ_WIDTH, PROJ_HEIGHT,
									SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL)))
		return (EXIT_FAILURE);

	if (!(renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)))
		return (EXIT_FAILURE);


	handle_events(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	map = generate_maze(INITIAL_MAZE_SIZE, &player, renderer);


	while (TRUE)
	{
		handle_events(renderer);
		SDL_Delay(100);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	ft_arrdel(&map);
	return (EXIT_SUCCESS);
}
