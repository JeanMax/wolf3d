/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_player.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/26 04:01:24 by mc                #+#    #+#             */
/*   Updated: 2017/04/08 22:28:38 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** todo
*/

#include "sdl.h"

/**
** update the coordinates of the given player
** @param: the t_player *ME to update
*/
void update_player(t_player *me) //TODO
{
	if (me->action & A_UP)
		me->coord.y -= SPEED_PER_FRAME * ((me->status & S_RUN) ? RUN_BONUS : 1);
	if (me->action & A_DOWN)
		me->coord.y += SPEED_PER_FRAME * ((me->status & S_RUN) ? RUN_BONUS : 1);

	if (me->action & A_RIGHT)
		me->coord.x += SPEED_PER_FRAME;
	if (me->action & A_LEFT)
		me->coord.x -= SPEED_PER_FRAME;

	if (me->action & A_ROLL)
		me->angle += ROLL_PER_FRAME;
	if (me->action & A_UNROLL)
		me->angle -= ROLL_PER_FRAME;

	if (me->angle < 0)
		me->angle += 2 * M_PI;
	if (me->angle >= 2 * M_PI)
		me->angle -= 2 * M_PI;
}
