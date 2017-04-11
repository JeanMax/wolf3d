/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maze.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:09:15 by mc                #+#    #+#             */
/*   Updated: 2017/04/11 16:30:21 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAZE_H
# define MAZE_H

# include <time.h> //bonus
# include <stdlib.h>

# include "wolf3d.h"

# define MAP_CHAR(MAP, X, Y) (*(*((char **)(MAP) + (int)(Y)) + (int)(X)))

# ifdef DEBUG_MODE
#  define MAZE_LATENCY 1e3
# else
#   define MAZE_LATENCY 1e4
# endif

/*
** draw_map.c
*/
void draw_map(t_context *context);

#endif
