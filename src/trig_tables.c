/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trig_tables.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/19 21:14:47 by mc                #+#    #+#             */
/*   Updated: 2017/04/20 20:56:05 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** we can store cos/sin/tan values in some arrays, since our maximal angle
** precision can be easily calculed (cf. wolf3d.h)
*/

#include "wolf3d.h"

void init_tables(t_context *context)
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
