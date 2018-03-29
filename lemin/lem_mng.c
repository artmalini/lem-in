/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_mng.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 16:58:10 by amakhiny          #+#    #+#             */
/*   Updated: 2018/03/20 16:58:18 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem_in.h"

int			lem_symbols(int c)
{
	if (c > 32 && c < 127)
		return (1);
	return (0);
}

void		lem_split_free(char **str)
{
	int		i;

	i = -1;
	while (str[++i])
		ft_memdel((void**)&str[i]);
	ft_memdel((void**)&str);
}

int			valid_rooms(t_data *lst)
{
	int		first;
	int		last;
	t_room	*room;

	first = 0;
	last = 0;
	while (lst)
	{
		room = (t_room *)lst->data;
		if (room->flag == 1)
			first++;
		if (room->flag == 2)
			last++;
		lst = lst->next;
	}
	return (first == 1 && last == 1 ? 1 : 0);
}

t_room		*place_to_chamber(t_data *rooms, int direct)
{
	int		val;
	t_room	*room;

	val = 0;
	while (rooms && !val)
	{
		room = (t_room *)rooms->data;
		val = room->flag == direct ? 1 : 0;
		rooms = rooms->next;
	}
	return (val ? room : 0);
}

t_ant		*lem_ant_struct(t_data *room, int ants, int str, int fin)
{
	int		i;
	t_ant	*insects;

	i = 0;
	if (!(insects = (t_ant *)malloc(sizeof(t_ant) * ants)))
		lem_error();
	while (str < fin)
	{
		insects[i].id = str + 1;
		insects[i].ant_turn = 0;
		insects[i].last = NULL;
		insects[i].room = place_to_chamber(room, 1);
		i++;
		str++;
	}
	return (insects);
}
