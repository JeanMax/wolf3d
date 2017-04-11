/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d_test.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/10 23:48:07 by mc                #+#    #+#             */
/*   Updated: 2017/04/10 23:48:28 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d_test.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/31 00:30:12 by mc                #+#    #+#             */
/*   Updated: 2017/04/09 17:33:38 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_TEST
# define WOLF3D_TEST

# include "minunit.h"

# include "wolf3d.h"
# include "maze.h"
# include "raycaster.h"
# include "sdl.h"

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include <limits.h>


/*
** maze
*/
void test_generate_maze(void);


/*
** raycaster
*/
void test_get_wall_coord_3x3(void);
void test_get_wall_coord_5x5(void);


/*
** sdl
*/
void test_move_player(void);

#endif