/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists_mng.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 16:24:05 by amakhiny          #+#    #+#             */
/*   Updated: 2018/03/20 16:24:13 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem_in.h"

void		lem_lst_foreach(t_data *lst, void (*f)())
{
	t_data		*tmp;

	tmp = lst;
	while (tmp)
	{
		(*f)(tmp->data);
		tmp = tmp->next;
	}
}

void		lem_lst_data_free(t_data *first)
{
	t_data	*tmp;

	tmp = first;
	while (first)
	{
		tmp = first;
		first = first->next;
		free(tmp);
	}
}

void		free_rooms(void *data)
{
	t_room	*room;

	room = (t_room *)data;
	ft_strdel(&room->name);
	lem_lst_data_free(room->paths);
	free(data);
	data = NULL;
	return ;
}

void		free_paths(void *data)
{
	t_links	*path;

	path = (t_links *)data;
	ft_strdel(&path->link1);
	ft_strdel(&path->link2);
	free(data);
	data = NULL;
	return ;
}

void		lem_free(t_game *game)
{
	if (game)
	{		
		if (game->ant_data)
			ft_memdel((void **)&game->ant_data);
		if (game->room_data)
		{
			lem_lst_foreach(game->room_data, free_rooms);
			lem_lst_data_free(game->room_data);
		}
		if (game->path_data)
		{
			lem_lst_foreach(game->path_data, free_paths);
			lem_lst_data_free(game->path_data);
		}
		ft_memdel((void **)&game);
	}
}
