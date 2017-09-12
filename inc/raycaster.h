/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:09:15 by mc                #+#    #+#             */
/*   Updated: 2017/09/12 17:56:32 by mcanal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCASTER_H
# define RAYCASTER_H

# include "wolf3d.h"

# define TEX_SKY_WIDTH  5760
# define TEX_SKY_HEIGHT 300

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
