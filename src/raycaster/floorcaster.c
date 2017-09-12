/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floorcaster.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/13 22:27:39 by mc                #+#    #+#             */
/*   Updated: 2017/09/12 17:42:34 by mcanal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycaster.h"

static void	draw_floor_pixel(t_context *context, t_point *floor_coord,
							int x, int y)
{
	if (!in_map(context->map, floor_coord->x, floor_coord->y)
		|| x >= PROJ_WIDTH || y >= PROJ_HEIGHT)
		return ;
	context->screen_pixels[x + y * PROJ_WIDTH] =
		*((t_uint *)context->surfaces[MAP_CHAR(context->map->ptr,
												floor_coord->x,
												floor_coord->y) == EXIT ?
										SUR_FLOOR_EXIT : SUR_FLOOR]->pixels
			+ ((t_uint)(fmod(floor_coord->x, TILE_SIZE))
				+ TILE_SIZE * (t_uint)(fmod(floor_coord->y, TILE_SIZE))));
}

/*
** because you can't fly, we made this floor
** well, here we'll just draw a strip at the give screen X position
** right under the WALL
*/

void		floorcaster(t_context *context, int x, t_polar_point *wall)
{
	int				y;
	t_polar_point	floor;
	int				floor_height;

	floor_height = (PROJ_HEIGHT - \
					(int)(WALL_HEIGHT * PROJ_DIST / wall->dist))
		/ 2;
	floor.angle = RAD_TO_ANGLE(atan(wall->dist / (double)PLAYER_HEIGHT));
	y = PROJ_HEIGHT - floor_height;
	while (y < PROJ_HEIGHT)
	{
		if ((floor.dist =
			context->tan_table[floor.angle] * (double)PLAYER_HEIGHT) > 0)
		{
			floor.coord.x =
				context->me.coord.x +
				(wall->coord.x - context->me.coord.x) * floor.dist / wall->dist;
			floor.coord.y =
				context->me.coord.y +
				(wall->coord.y - context->me.coord.y) * floor.dist / wall->dist;
			draw_floor_pixel(context, &floor.coord, x, y);
		}
		y++;
		floor.angle -= 3;
	}
}
