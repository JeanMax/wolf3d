/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:09:15 by mc                #+#    #+#             */
/*   Updated: 2017/04/20 14:03:56 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCASTER_H
# define RAYCASTER_H

# include "wolf3d.h"

# define TEX_SKY_WIDTH  5760 // (PROJ_WIDTH * 2 * M_PI / FOV)
# define TEX_SKY_HEIGHT 300 // (PROJ_HEIGHT / 2)

# define CLR_WALL_NORTH 0xff
# define CLR_WALL_EAST  0xbb
# define CLR_WALL_SOUTH 0x99
# define CLR_WALL_WEST  0xdd

/*
** skybox.c
*/
void skybox(t_context *context);

/*
** floorcaster.c
*/
void floorcaster(t_context *context, int x, t_polar_point *wall);

#endif
