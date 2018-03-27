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
		ft_putstr("ERROR\n");
	exit(1);
}

void		lem_error(void)
{
	ft_putstr("ERROR\n");
	exit(1);
}
