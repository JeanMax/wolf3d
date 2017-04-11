/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mc </var/spool/mail/mc>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 00:30:53 by mc                #+#    #+#             */
/*   Updated: 2017/04/11 15:59:01 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** just a main... this is important to keep this file the lightest possible,
** because it won't linked in the test executable
*/

#include "wolf3d.h"

static int usage_error(char *exec_name, int fd)
{
	ft_putstr_fd(CLR_RED "Usage: " CLR_WHITE, fd);
	ft_putstr_fd(exec_name, fd);
	ft_putstr_fd(" [" CLR_GREEN "MAZE_SIZE" CLR_WHITE "]" CLR_RESET "\n", fd);

	if (fd == STDERR_FILENO)
	{
		ft_putstr_fd("Try '", fd);
		ft_putstr_fd(exec_name, fd);
		ft_putstr_fd(" --help' for more information.\n", fd);
		return (EXIT_FAILURE);
	}

	ft_putstr_fd("The map generated will be a square shapped maze "	\
				 "(width/height = " CLR_GREEN "MAZE_SIZE" CLR_RESET ")" \
				 "\n\t", fd);

	ft_putnbr_fd(MIN_MAZE_SIZE, fd);
	ft_putstr_fd(" >= " CLR_GREEN "MAZE_SIZE" CLR_RESET " >= ", fd);
	ft_putnbr_fd(MAX_MAZE_SIZE, fd);
	ft_putstr_fd(" - default: ", fd);
	ft_putnbr_fd(INITIAL_MAZE_SIZE, fd);
	ft_putstr_fd("\n", fd);

	return (EXIT_SUCCESS);
}

int		main(int ac, char **av)
{
	t_context context;
	t_uint maze_size;
	char *s;

	if (ac == 1)
		maze_size = INITIAL_MAZE_SIZE;
	else if (ac == 2)
	{
		if (!ft_strcmp(*(av + 1), "--help") || !ft_strcmp(*(av + 1), "-h"))
				return (usage_error(*av, STDOUT_FILENO));

		s = *(av + 1);
		while (*s)
			if (!ft_isdigit(*s++))
				return (usage_error(*av, STDERR_FILENO));

		maze_size = ft_atoi(*(av + 1));
		if (maze_size < MIN_MAZE_SIZE || maze_size > MAX_MAZE_SIZE)
			return (usage_error(*av, STDERR_FILENO));
	}
	else
		return (usage_error(*av, STDERR_FILENO));

	if (!init(&context))
		return (EXIT_FAILURE);

	game_loop(&context, maze_size);

	return (finit(&context) ? EXIT_SUCCESS : EXIT_FAILURE);
}
