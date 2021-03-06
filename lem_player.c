/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_player.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 16:36:04 by amakhiny          #+#    #+#             */
/*   Updated: 2018/03/20 16:36:06 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem_in.h"

int			lem_last_ant(t_ant *ants, int numbrs)
{
	int		i;

	i = -1;
	while (++i < numbrs && ants != 0)
	{
		if (ants[i].room->flag != 2)
			return (0);
	}
	return (1);
}

int			check_turn(t_ant *ants)
{
	t_room	*path;
	t_data	*list;

	if (ants->room->flag == 2)
		return (0);
	list = ants->room->paths;
	while (list && !ants->ant_turn)
	{
		path = (t_room *)list->data;
		if (path->flag == 2 || (!path->ant && path !=
			ants->last && path->flag != 1))
			return (1);
		list = list->next;
	}
	return (0);
}

void		lem_role(t_game *game, int i, int turn, int temp_nbr)
{
	while (turn && !lem_last_ant(game->ant_data, temp_nbr))
	{
		i = -1;
		turn = 0;
		while (++i < temp_nbr)
		{
			if (check_turn(game->ant_data + i))
			{
				turn = 1;
				game->ant_data += i;
				ants_role(game);
				game->ant_data -= i;
			}
		}
		ft_putchar('\n');
	}
	if (game->ant_data)
		ft_memdel((void **)&game->ant_data);
}

void		lem_str_play(t_game *game, int temp_nbr, int tm, int nbr)
{
	int		i;
	int		turn;

	i = 0;
	turn = 1;
	while (tm < game->total)
	{
		if (nbr >= SWARM)
		{
			temp_nbr = SWARM;
			nbr -= SWARM;
			game->ant_data = l_ant_s(game->room_data, SWARM, tm, tm + SWARM);
			tm += SWARM;
		}
		else
		{
			game->ant_data = l_ant_s(game->room_data, nbr, tm, game->total);
			temp_nbr = nbr;
			tm = game->total;
		}
		ants_ready(game->ant_data, temp_nbr);
		lem_role(game, i, turn, temp_nbr);
	}
}

void		lem_player(t_game *game)
{
	int		temp_nbr;
	int		nbr;
	int		tmp_total;

	temp_nbr = 0;
	tmp_total = 0;
	nbr = game->total;
	lem_str_play(game, temp_nbr, tmp_total, nbr);
	if (game->visual)
	{
		ft_putstr("\033[32m");
		ft_putstr("All ants swarm leaving their nest successfully!\033[0m\n");
	}
}
