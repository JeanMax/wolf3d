/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maze.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:09:15 by mc                #+#    #+#             */
/*   Updated: 2017/04/20 14:06:02 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAZE_H
# define MAZE_H

# include <time.h> //bonus
# include <stdlib.h>

# include "wolf3d.h"

# define MAZE_CHAR(MAP, X, Y) (*(*((char **)(MAP) + (int)(Y)) + (int)(X)))

# ifdef DEBUG_MODE
#  define MAZE_LATENCY 1e3
# else
#   define MAZE_LATENCY 1e4
# endif

/*
** maze_helpers.c
*/
t_bool touch_one_empty_tile(t_arr *map, t_point *p);
void free_string(void *s, size_t n);
void *cpy_wall(void *dest, const void *src, size_t n);

#endif
