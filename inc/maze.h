/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maze.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:09:15 by mc                #+#    #+#             */
/*   Updated: 2017/03/25 19:37:07 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAZE_H
# define MAZE_H

# include <time.h> //bonus
# include <stdlib.h>

# include "wolf3d.h"

#define MAP_CHAR(MAP, X, Y) (*(*((char **)(MAP) + (int)(Y)) + (int)(X)))

/*
** draw_map.c
*/
void draw_map(t_arr *map, t_point *start, t_point *exit, SDL_Renderer *renderer);

#endif
