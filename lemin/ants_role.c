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

/*
* [91m - Red [92m - Green [93m - Yellow
* [94m - Blue [95m Purple [96m - Cyan
*/
char	*color(int x)
{
	char	*rgb[6];

	rgb[0] = "\x1B[91m";
	rgb[1] = "\x1B[92m";
	rgb[2] = "\x1B[93m";
	rgb[3] = "\x1B[94m";
	rgb[4] = "\x1B[95m";
	rgb[5] = "\x1B[96m";
	return (rgb[x]);
}

void	print_ant(t_ant *ant, int vis)
{
	if (vis == 1)
		ft_putstr(color(ant->id % 6));
	ft_putchar('L');
	ft_putnbr(ant->id);
	ft_putchar('-');
	if (vis == 1 && ant->room->flag == 2)
			ft_putstr("\x1B[90m");
	ft_putstr(ant->room->name);
	ft_putchar(' ');
	if (vis == 1)
		ft_putstr("\033[0m");
	return ;
}


void		move_ants(t_ant *ant, t_room *room, int vis)
{
	ant->room->ant = 0;
	ant->last = ant->room;
	ant->room = room;
	ant->room->ant = 1;
	print_ant(ant, vis);
	return ;
}

int			ant_cell(void *room, int last)
{
	t_data	*search;
	t_room	*current;
	int		find;
	int		old_find;

	current = (t_room *)room;
	find = 2147483647;
	if (current->flag == last)
		return (-1);
	if (current->full)
		return (-2);
	current->full = 1;
	search = current->paths;
	while (search)
	{
		if ((old_find = ant_cell(search->data, last)) < find && old_find != -2)
			find = 1 + old_find;
		search = search->next;
	}
	current->full = 0;
	return (find == 2147483647 ? -1 : find);
}

void		ants_role(t_game *game)
{
	int		result;
	int		calc;
	t_data	*ls;
	t_room	*tmp;
	t_room	*cell;

	calc = 2147483647;
	ls = game->ant_data->room->paths;
	while (ls)
	{
		tmp = (t_room *)ls->data;
		if (tmp != game->ant_data->last && (tmp->flag == 2 || !tmp->ant))
		{
			result = ant_cell(tmp, 2);
			if (result < calc && result > -2)
			{
				calc = result;
				cell = tmp;
			}
		}
		ls = ls->next;
	}
	if (calc < 2147483647)
		move_ants(game->ant_data, cell, game->visual);
}