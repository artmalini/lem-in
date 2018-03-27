/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ants_role.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 16:47:08 by amakhiny          #+#    #+#             */
/*   Updated: 2018/03/20 16:47:13 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem_in.h"

void		print_ant(t_ant *ant)
{
	ft_putchar('L');
	ft_putnbr(ant->id);
	ft_putchar('-');
	ft_putstr(ant->room->name);
	ft_putchar(' ');
	return ;
}


void		move_ants(t_ant *ant, t_room *room)
{
	ant->room->ant = 0;
	ant->last = ant->room;
	ant->room = room;
	ant->room->ant = 1;
	print_ant(ant);
	return ;
}

int			find_room(void *room, int last)
{
	t_data	*search;
	t_room	*current;
	int		find;
	int		old_find;

	current = (t_room *)room;
	find = 2147483647;
	if (current->flag == last)
		return (0);
	if (current->full)
		return (-1);
	current->full = 1;
	search = current->paths;
	while (search)
	{
		if ((old_find = find_room(search->data, last)) < find && old_find != -1)
			find = 1 + old_find;
		search = search->next;
	}
	current->full = 0;
	return (find != 2147483647 ? find : -1);
}

void		ants_role(t_game *game)
{
	int		result;
	int		calc;
	t_data	*ls;
	t_room	*tmp;
	t_room	*next;

	calc = 2147483647;
	ls = game->ant_data->room->paths;
	while (ls)
	{
		tmp = (t_room *)ls->data;
		if (tmp != game->ant_data->last && (tmp->flag == 2 || !tmp->ant))
		{
			result = find_room(tmp, 2);
			if (result < calc && result > -1)
			{
				calc = result;
				next = tmp;
			}
		}
		ls = ls->next;
	}
	if (calc < 2147483647)
		move_ants(game->ant_data, next);
}
