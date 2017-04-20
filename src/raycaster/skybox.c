/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/13 22:02:49 by mc                #+#    #+#             */
/*   Updated: 2017/04/20 00:58:23 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycaster.h"

//TODO: centralise define from raycaster.c
#define DOUBLE_PRECISION (1e-6)
#define ZERO(X) ((X) > -DOUBLE_PRECISION * 2 && (X) < DOUBLE_PRECISION * 2)

static void copy_sky_pixels(t_context *context, SDL_Rect *dst, SDL_Rect *src)
{
	SDL_Rect src_cpy;
	SDL_Rect dst_cpy;

	ft_memcpy(&src_cpy, src, sizeof(SDL_Rect));
	ft_memcpy(&dst_cpy, dst, sizeof(SDL_Rect));
	while (src_cpy.h)
	{
		while (src_cpy.w)
		{
			context->screen_pixels[dst_cpy.x + dst_cpy.y * PROJ_WIDTH] \
				= *((t_uint *)context->surfaces[SUR_SKY]->pixels	\
					+ (src_cpy.x % TEX_SKY_WIDTH) + TEX_SKY_WIDTH * src_cpy.y);
			dst_cpy.x++;
			src_cpy.x++;
			src_cpy.w--;
		}
		src_cpy.w = src->w;
		src_cpy.x = src->x;
		dst_cpy.x = dst->x;
		dst_cpy.y++;
		src_cpy.y++;
		src_cpy.h--;
	}
}

void skybox(t_context *context)
{
	SDL_Rect src;
	SDL_Rect dst;

	dst.x = 0;
	dst.y = 0;
	dst.w = PROJ_WIDTH;
	dst.h = PROJ_HEIGHT / 2;

	src.x = (int)(TEX_SKY_WIDTH								\
				  * (double)(mod2pi(-(context->me.angle - FOV / 2)) \
					 / (double)(2 * PI)));
	src.y = 0;
	src.w = dst.w;
	src.h = dst.h;

	copy_sky_pixels(context, &dst, &src);
}
