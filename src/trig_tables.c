/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trig_tables.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcanal <mc.maxcanal@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/12 17:04:10 by mcanal            #+#    #+#             */
/*   Updated: 2017/09/12 17:04:24 by mcanal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** we can store cos/sin/tan values in some arrays, since our maximal angle
** precision can be easily calculed (cf. wolf3d.h)
*/

#include "wolf3d.h"

void	init_tables(t_context *context)
{
	int i;

	i = 0;
	while (i < ANGLE_MAX)
	{
		context->cos_table[i] = cos(ANGLE_TO_RAD(i));
		context->sin_table[i] = sin(ANGLE_TO_RAD(i));
		context->tan_table[i] = tan(ANGLE_TO_RAD(i));
		i++;
	}
}
