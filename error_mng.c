/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_mng.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 15:21:50 by amakhiny          #+#    #+#             */
/*   Updated: 2018/03/20 15:21:57 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem_in.h"

void		param_error(char *arg, char *argv)
{
	if (arg && argv)
	{
		display_header();
		ft_putstr("ERROR: Argument ");
		ft_putstr(arg);
		ft_putstr(" is invalid!\n");
		ft_putstr("   You can choose folowing input:\n");
		ft_putstr(" \033[33m./lem-in -v < map_name\033[0m");
		ft_putstr("   To show Lemin in color\n");
		ft_putstr(" \033[33m./lem-in -h < map_name\033[0m");
		ft_putstr("   To hide map from output\n");
		ft_putstr("\033[33m ");
		ft_putstr(argv);
		ft_putstr(" < map_name\033[0m      To run without arguments\n");
	}
	exit(1);
}

void		lem_error(void)
{
	ft_putstr("ERROR\n");
	exit(1);
}
