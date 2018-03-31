/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 15:08:17 by amakhiny          #+#    #+#             */
/*   Updated: 2018/03/20 15:08:21 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem_in.h"

int			main(int argc, char **argv)
{
	t_game	*game;

	if (!(game = (t_game *)malloc(sizeof(t_game))))
		lem_error();
	lem_struct(game);
	arg_parser(game, argc, argv);
	if (!lem_read_map(game))
	{
		lem_free(game);
		lem_error();
	}
	lem_set_path(game);
	lem_not_cmplinks(game);
	if (game->visual == 1)
		display_header();
	if (game->map_hide == 0)
		lem_print_map(game);
	if (game->visual == 1)
		display_header_ant(game->map_hide);
	lem_player(game);
	lem_free(game);
	return (0);
}
