/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:09:15 by mc                #+#    #+#             */
/*   Updated: 2017/04/18 18:07:01 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SDL_H
# define SDL_H

# include <time.h> //bonus
# include <stdlib.h>

# include "wolf3d.h"

/*
** update_player.c
*/
void move_player(t_context *context, double angle, double distance);


/*
** event_handlers-move.c
*/
t_bool handler_move_up(t_context *context, int key, t_uint type);
t_bool handler_move_down(t_context *context, int key, t_uint type);
t_bool handler_move_left(t_context *context, int key, t_uint type);
t_bool handler_move_right(t_context *context, int key, t_uint type);

/*
** event_handlers-roll.c
*/
t_bool handler_roll(t_context *context, int key, t_uint type);
t_bool handler_unroll(t_context *context, int key, t_uint type);


/*
**  event_handlers-status.c
*/
t_bool handler_run(t_context *context, int key, t_uint type);
t_bool handler_map(t_context *context, int key, t_uint type);

#endif
