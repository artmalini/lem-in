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

int		fin_lem_review2(t_data *path, char *link1)
{
	while (path)
	{
		if ((!ft_strcmp(((t_links *)path->data)[0].link1, link1)))
			return (1);
		path = path->next;
	}
	return (0);
}

int		lem_review2(t_game *game, char *link1, int flag)
{
	t_data	*path;

	path = game->path_data;
	if (flag == 1 || flag == 2)
		return (1);
	else if (fin_lem_review2(path, link1))
		return (1);
	return (0);
}

int		fin_lem_review1(t_data *path, char *link2)
{
	while (path)
	{
		if ((!ft_strcmp(((t_links *)path->data)[0].link2, link2)))
			return (1);
		path = path->next;
	}
	return (0);
}

int		lem_review1(t_game *game, char *link2, int flag)
{
	t_data	*path;

	path = game->path_data;
	if (flag == 1 || flag == 2)
		return (1);
	else if (fin_lem_review1(path, link2))
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
				lem_review1(game, links->link2, room->flag))
					room->paths = ft_lem_push(room->paths,
						room_name(game->room_data, links->link2));
			if (!ft_strcmp(links->link2, room->name) &&
				lem_review2(game, links->link1, room->flag))
					room->paths = ft_lem_push(room->paths,
						room_name(game->room_data, links->link1));
			tmp_path = tmp_path->next;
		}
		tmp_room = tmp_room->next;
	}
}
