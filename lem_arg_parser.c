/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_arg_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/31 11:54:38 by amakhiny          #+#    #+#             */
/*   Updated: 2018/03/31 11:54:44 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem_in.h"

int			ants_ready(t_ant *ants, int numbrs)
{
	int		i;

	i = -1;
	while (++i < numbrs)
		ants[i].ant_turn = 0;
	return (1);
}

void		lem_struct(t_game *game)
{
	game->total = 0;
	game->visual = 0;
	game->map_hide = 0;
	game->ant_data = NULL;
	game->room_data = NULL;
	game->path_data = NULL;
	game->map = NULL;
}

int			lem_read_map(t_game *game)
{
	game->total = lem_get_ants();
	if (game->total == 0 || lem_get_map(game, 3, 0, 0))
		return (0);
	if (!valid_rooms(game->room_data))
		return (0);
	return (1);
}

void		lem_not_cmplinks(t_game *game)
{
	t_links	*links;
	t_data	*tmp_room;
	t_data	*tmp_path;

	tmp_room = game->room_data;
	while (tmp_room)
	{
		tmp_path = game->path_data;
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

void		arg_parser(t_game *game, int argc, char **argv)
{
	int		i;

	i = 1;
	while (--argc)
	{
		if (ft_strequ(argv[i], "-v"))
			game->visual = 1;
		else if (ft_strequ(argv[i], "-h"))
			game->map_hide = 1;
		else
			param_error(argv[i], argv[0]);
		i++;
	}
}
