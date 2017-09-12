/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maze.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:09:15 by mc                #+#    #+#             */
/*   Updated: 2017/09/12 17:54:45 by mcanal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAZE_H
# define MAZE_H

# include <time.h>
# include <stdlib.h>

# include "wolf3d.h"

# define MAZE_CHAR(MAP, X, Y) (*(*((char **)(MAP) + (int)(Y)) + (int)(X)))

# define MAZE_LATENCY 1e4

/*
** maze_helpers.c
*/
t_bool	touch_one_empty_tile(t_arr *map, t_point *p);
void	free_string(void *s, size_t n);
void	*cpy_wall(void *dest, const void *src, size_t n);

#endif
