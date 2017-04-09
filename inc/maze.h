/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maze.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:09:15 by mc                #+#    #+#             */
/*   Updated: 2017/03/28 20:45:30 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAZE_H
# define MAZE_H

# include <time.h> //bonus
# include <stdlib.h>

# include "wolf3d.h"

# define MAP_CHAR(MAP, X, Y) (*(*((char **)(MAP) + (int)(Y)) + (int)(X)))
/* # define MAZE_LATENCY 2e4 */
# define MAZE_LATENCY 1e3

/*
** draw_map.c
*/
void draw_map(t_context *context);

#endif
