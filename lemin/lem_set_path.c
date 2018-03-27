/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_set_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 16:32:17 by amakhiny          #+#    #+#             */
/*   Updated: 2018/03/20 16:32:19 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem_in.h"

int		best_path(t_data *tmp_room, char *link)
{
	int		i;
	t_room	*room;

	i = 0;
	while (tmp_room != 0 && !i)
	{
		room = (t_room *)tmp_room->data;
		if (!ft_strcmp(room->name, link))
			i = (room->flag == 2) ? 1 : 0;
		tmp_room = tmp_room->next;
	}
	return (i);
}

int		fin_lem_review(t_data *path, t_data *rooms, t_game *game, char *link1, int i)
{
	while (++i < 2)
	{
		while (rooms)
		{
			while (path)
			{
				if (!ft_strcmp(((t_links *)path->data)[0].link1, link1))
				{
					link1 = ((t_links *)path->data)[0].link2;
					if (best_path(game->room_data,
						((t_links *)path->data)[0].link2))
						return (1);
					break ;
				}
				path = path->next;
			}
			path = game->path_data;
			rooms = rooms->next;
		}
		rooms = game->room_data;
	}
	return (0);
}

int		lem_review(t_game *game, char *link1)
{
	t_data	*path;
	t_data	*rooms;

	rooms = game->room_data;
	path = game->path_data;
	if (fin_lem_review(path, rooms, game, link1, -1))
		return (1);
	return (0);
}

void		lem_set_path(t_game *game)
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
			if (!ft_strcmp(links->link1, room->name) &&
				lem_review(game, links->link1))
					room->paths = ft_lem_push(room->paths,
						room_name(game->room_data, links->link2));
			if (!ft_strcmp(links->link2, room->name) &&
				lem_review(game, links->link2))
					room->paths = ft_lem_push(room->paths,
						room_name(game->room_data, links->link1));
			tmp_path = tmp_path->next;
		}
		tmp_room = tmp_room->next;
	}
}
