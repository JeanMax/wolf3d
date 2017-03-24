/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 00:30:53 by mc                #+#    #+#             */
/*   Updated: 2017/03/24 20:46:26 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "wolf3d.h"

int		main(void)
{
	/* SDL_Event	event; */
    /* SDL_Window	*window; */
	t_arr       *map;
	t_player    player;

	ft_bzero(&player, sizeof(t_player));

	map = generate_maze(INITIAL_MAZE_SIZE, &player);

	/* if (SDL_Init(SDL_INIT_VIDEO) < 0) */
	/* 	return (EXIT_FAILURE); */

	/* if (!(window = SDL_CreateWindow("zboub", */
	/* 								SDL_WINDOWPOS_CENTERED, */
	/* 								SDL_WINDOWPOS_CENTERED, */
	/* 								PROJ_WIDTH, PROJ_HEIGHT, */
	/* 								SDL_WINDOW_SHOWN))) */
	/* 	return (EXIT_FAILURE); */


	/* do { */
	/* 	SDL_WaitEvent(&event); */
	/* } while (event.type != SDL_QUIT); */

	/* SDL_DestroyWindow(window); */
	/* SDL_Quit(); */

	ft_arrdel(&map);
	return (EXIT_SUCCESS);
}
