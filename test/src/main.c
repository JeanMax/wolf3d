/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minunit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcanal <zboub@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/15 11:32:38 by mcanal            #+#    #+#             */
/*   Updated: 2017/04/14 17:26:15 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d_test.h"

static void all_suites(char *search)
{
	MU_TEST_SUITE suite_maze = {
		{test_generate_maze, "generate_maze"},
		//TODO: test if we can exit maze
	};

	MU_TEST_SUITE suite_raycaster = {
		{test_get_wall_coord_3x3, "get_wall_coord_3x3"},
		{test_get_wall_coord_5x5, "get_wall_coord_5x5"},
		{test_get_floor_coord, "get_floor_coord"},
	};

	MU_TEST_SUITE suite_sdl = {
		{test_move_player, "move_player"},
	};

	if (search) {
		int test_run_before = g_tests_run; //hmmm...

		MU_RUN_TEST_FROM_SUITE(search, suite_maze);
		MU_RUN_TEST_FROM_SUITE(search, suite_raycaster);
		MU_RUN_TEST_FROM_SUITE(search, suite_sdl);

		if (test_run_before == g_tests_run) {
			fprintf(stderr, "+ Test %s: "CLR_RED"not found\n"CLR_RESET, search);
			exit(EXIT_FAILURE);
		}
	} else {
		MU_RUN_SUITE(suite_maze, "maze");
		MU_RUN_SUITE(suite_raycaster, "raycaster");
		MU_RUN_SUITE(suite_sdl, "sdl");
	}
}


MU_HAI(); //yep, this is really ugly :/

int main(int ac, char **av) //TODO: --help
{
	setbuf(stdout, NULL); /* TODO: find a better place for this */

	if (ac == 1) {
		all_suites(NULL);
	} else {
		while (*++av) {
			all_suites(*av);
		}
		printf("\n");
	}

	MU_KTHXBYE();
	return EXIT_SUCCESS;
}
