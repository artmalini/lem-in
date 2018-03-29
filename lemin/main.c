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

int			main(int argc, char **argv)
{
	if (argc < 1)
		lem_error();
	else
		lem_parse(argc, argv);
	//system("leaks lem-in");
	return (0);
}
