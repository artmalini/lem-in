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

void		lem_struct(t_game *game)
{
	game->total = 0;
	game->done = 0;
	game->ant_data = NULL;
	game->room_data = NULL;
	game->path_data = NULL;
	game->map = NULL;
}

int		lem_read_map(t_game *game)
{
	game->total = lem_get_ants();
	if (game->total == 0 || lem_get_map(game, 3))
		return (0);
	if (!valid_rooms(game->room_data))
		return (0);
	game->ant_data = lem_ant_struct(game->room_data, game->total);
	return (1);
}


void		lem_print_map(t_game *game)
{
	ft_putnbr(game->total);
	ft_putchar('\n');
	ft_putstr(game->map);
	ft_putchar('\n');
	return ;
}

void		lem_not_cmplinks(t_game *game)
{
	t_room	*room;
	t_links	*links;
	t_data	*tmp_room;
	t_data	*tmp_path;

	tmp_room = game->room_data;
	while (tmp_room)
	{
		tmp_path = game->path_data;
		room = (t_room *)tmp_room->data;
		while (tmp_path)
		{
			links = (t_links *)tmp_path->data;
			if (!ft_strcmp(links->link1, links->link2))
				lem_error();
			tmp_path = tmp_path->next;
		}
		tmp_room = tmp_room->next;
	}
}

void		lem_parse(int argc, char **argv)
{
	t_game	*game;

	while (--argc)
		param_error(argv[1], argv[0]);
	if(!(game = (t_game *)malloc(sizeof(t_game))))
		lem_error();
	lem_struct(game);
	if (!lem_read_map(game))
	{
		lem_free(game);
		lem_error();
	}
	lem_set_path(game);
	lem_not_cmplinks(game);
	lem_print_map(game);
	lem_player(game);
	lem_free(game);
}

int			main(int argc, char **argv)
{
	if (argc < 1)
		lem_error();
	else
		lem_parse(argc, argv);
	//system("leaks lem-in");
	return (0);
}
