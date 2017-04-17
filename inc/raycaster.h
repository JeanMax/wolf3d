/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:09:15 by mc                #+#    #+#             */
/*   Updated: 2017/04/15 13:16:46 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCASTER_H
# define RAYCASTER_H

# include "wolf3d.h"

void skybox(t_context *context);
void floorcaster(t_context *context, \
				 double wall_dist, double angle, t_point *wall_coord, \
				 int x);

double get_floor_coord(t_point *dst, t_context *context, \
					   double angle, double vert_angle);

#endif
