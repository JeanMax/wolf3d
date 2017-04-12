/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/09 13:18:26 by mc                #+#    #+#             */
/*   Updated: 2017/04/12 14:37:27 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "wolf3d.h"

//TODO: same macros as raycaster.c, move them to some header

#define MAP_CHAR(MAP, X, Y) (*(*((char **)(MAP) + (int)((Y) / TILE_SIZE)) + (int)((X) / TILE_SIZE)))

static t_bool load_textures(t_context *context)
{
	char *tex_path[] = { //TODO defines?
		TEX_DIR "/floor.bmp",
		TEX_DIR "/wall_west.bmp",
		TEX_DIR "/ceiling.bmp",
		TEX_DIR "/wall_north.bmp",
		TEX_DIR "/wall_south.bmp",
		TEX_DIR "/wall_east.bmp",
		TEX_DIR "/floor_exit.bmp",
	};
	int i;
	SDL_Surface *surface;

	i = 0;
	while (i < MAX_TEX)
	{
		if (!(surface = SDL_LoadBMP(tex_path[i])))
		{
			ft_putstr_fd(tex_path[i], STDERR_FILENO);
			ft_putendl_fd(": "CLR_RED"file not found"CLR_RESET, STDERR_FILENO);
			return (FALSE);
		}
		if (!(context->textures[i] \
			  = SDL_CreateTextureFromSurface(context->renderer, surface)))
		{
			ft_putstr_fd(tex_path[i], STDERR_FILENO);
			ft_putendl_fd(": "CLR_RED"not texturisable"CLR_RESET\
						  " (that's totally a word)", STDERR_FILENO);
			return (FALSE);
		}
		SDL_FreeSurface(surface);
		i++;
	}

	return (TRUE);
}

t_bool init(t_context *context) //TODO: error messages?
{
	ft_bzero(context, sizeof(t_context));

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return (FALSE);

	if (!(context->window = SDL_CreateWindow("zboub",
											 /* SDL_WINDOWPOS_UNDEFINED, */
											 /* SDL_WINDOWPOS_UNDEFINED, */
											 /* 0, 0, */
											 /* SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL))) */
									SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED,
									PROJ_WIDTH, PROJ_HEIGHT,
									SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL)))
		return (finit(context) && FALSE);

	if (!(context->renderer = SDL_CreateRenderer(context->window,
												 -1,
												 SDL_RENDERER_PRESENTVSYNC)))
		return (finit(context) && FALSE);

	if (!load_textures(context))
		return (finit(context) && FALSE);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(context->renderer, PROJ_WIDTH, PROJ_HEIGHT);

	SDL_SetRenderDrawColor(context->renderer, SDL_BLACK);
	SDL_RenderClear(context->renderer);

	return (TRUE);
}


t_bool finit(t_context *context)
{
	int i;

	i = 0;
	while (i < MAX_TEX)
	{
		if (context->textures[i])
			SDL_DestroyTexture(context->textures[i]);
		i++;
	}
	if (context->renderer)
		SDL_DestroyRenderer(context->renderer);
	if (context->window)
		SDL_DestroyWindow(context->window);
	SDL_Quit();
	if (context->map)
		ft_arrdel(&context->map);

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
