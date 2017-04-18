/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:09:15 by mc                #+#    #+#             */
/*   Updated: 2017/04/18 13:39:10 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCASTER_H
# define RAYCASTER_H

# include "wolf3d.h"

void skybox(t_context *context);
void floorcaster(t_context *context, double wall_dist, t_point *wall_coord, int x);

#endif
