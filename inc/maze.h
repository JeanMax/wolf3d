/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maze.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:09:15 by mc                #+#    #+#             */
/*   Updated: 2017/03/25 02:36:55 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAZE_H
# define MAZE_H

# include <time.h> //bonus
# include <stdlib.h>

# include "wolf3d.h"

#define MAP_CHAR(MAP, X, Y) (*(*((char **)(MAP) + (int)(Y)) + (int)(X)))

#define BLACK 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE
#define RED   0xff, 0x00, 0x00, SDL_ALPHA_OPAQUE
#define GREEN 0x00, 0xff, 0x00, SDL_ALPHA_OPAQUE
#define BLUE  0x00, 0x00, 0xff, SDL_ALPHA_OPAQUE
#define WHITE 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE

/*
** draw_map.c
*/
void draw_map(t_arr *map, t_point *start, t_point *exit, SDL_Renderer *renderer);

#endif
