/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 00:30:53 by mc                #+#    #+#             */
/*   Updated: 2017/04/09 13:22:15 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** just a main... this is important to keep this file the lightest possible,
** because it won't linked in the test executable
*/

#include "wolf3d.h"

int		main(void)
{
	t_context context;

	if (!init(&context))
		return (EXIT_FAILURE);

	game_loop(&context);

	return (finit(&context) ? EXIT_SUCCESS : EXIT_FAILURE);
}
