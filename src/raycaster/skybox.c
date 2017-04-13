/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc <mc.maxcanal@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/13 22:02:49 by mc                #+#    #+#             */
/*   Updated: 2017/04/13 22:03:55 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycaster.h"

//TODO: centralise define from raycaster.c
#define DOUBLE_PRECISION (1e-6)
#define ZERO(X) ((X) > -DOUBLE_PRECISION * 2 && (X) < DOUBLE_PRECISION * 2)

void skybox(t_context *context)
{
	SDL_Rect src;
	SDL_Rect dst;
	double overlap_ratio;

	src.y = 0;
	src.w = (int)(TEX_SKY_WIDTH * (FOV / (2 * M_PI)));
	src.h = TEX_SKY_HEIGHT;
	src.x = (int)(TEX_SKY_WIDTH											\
				* (mod2pi(-(context->me.angle - FOV / 2)) / (2 * M_PI)) \
				- src.w);
	overlap_ratio = mod2pi(context->me.angle + FOV / 2) / FOV;
	/* DEBUG(CLR_WHITE"me -> x:%f y:%f angle:%f\n"CLR_RESET,				\ */
		  /* context->me.coord.x, context->me.coord.y, context->me.angle); */
	if (overlap_ratio > 1 || ZERO(overlap_ratio) || ZERO(overlap_ratio - 1))
	{
		/* DEBUG(CLR_GREEN"src -> x:%d y:%d w:%d h:%d\n"CLR_RESET, \ */
			  /* src.x, src.y, src.w, src.h); */
		SDL_RenderCopy(context->renderer, context->textures[TEX_SKY],	\
					   &src, NULL);
	}
	else //overlap right/left of the texture
	{
		src.x = src.x + TEX_SKY_WIDTH;
		src.w *= overlap_ratio;

		dst.x = 0;
		dst.y = 0;
		dst.w = (int)(PROJ_WIDTH * overlap_ratio);
		dst.h = PROJ_HEIGHT;

		/* DEBUG(CLR_BLUE"src1 -> x:%d y:%d w:%d h:%d\n"CLR_RESET, \ */
			  /* src.x, src.y, src.w, src.h); */
		/* DEBUG(CLR_BLUE"dst1 -> x:%d y:%d w:%d h:%d\n"CLR_RESET, \ */
			  /* dst.x, dst.y, dst.w, dst.h); */
		SDL_RenderCopy(context->renderer, context->textures[TEX_SKY], \
					   &src, &dst);


		src.x = 0;
		src.w = (int)(src.w / overlap_ratio * (1 - overlap_ratio));

		dst.x = dst.w - 1;
		dst.w = PROJ_WIDTH - dst.x;

		/* DEBUG(CLR_CYAN"src2 -> x:%d y:%d w:%d h:%d\n"CLR_RESET, \ */
			  /* src.x, src.y, src.w, src.h); */
		/* DEBUG(CLR_CYAN"dst2 -> x:%d y:%d w:%d h:%d\n"CLR_RESET, \ */
			  /* dst.x, dst.y, dst.w, dst.h); */
		SDL_RenderCopy(context->renderer, context->textures[TEX_SKY], \
					   &src, &dst);
	}
	/* DEBUG(CLR_MAGENTA"ratio -> %f\n\n"CLR_RESET, \ */
			  /* overlap_ratio); */
}
