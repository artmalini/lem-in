/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_read_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 15:25:11 by amakhiny          #+#    #+#             */
/*   Updated: 2018/03/20 15:25:13 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/lem_in.h"

int		check_hash(char *output)
{
	if (*output != '#' || ft_strequ("##", output))
		return (0);
	else if (ft_strequ("##start", output))
		return (1);
	else if (ft_strequ("##end", output))
		return (2);
	else
		return (3);
}

void		map_check_hash(char **output, int *status)
{
	if (*status == 3)
		*status = check_hash(*output);
	else
		*status = *status;
}

char	*ft_joinstr(char *str1, char *str2)
{
	char	*str3;
	int		len1;
	int		len2;

	if (str1)
	{
		len1 = ft_strlen(str1);
		len2 = ft_strlen(str2);
		if (!(str3 = (char *)malloc(sizeof(str3) * ((len1 + len2) + 1))))
			return (NULL);
		*str3 = 0;
		str3 = ft_strcat(ft_strcat(str3, str1), str2);
		return (str3);
	}
	else
		return (ft_strdup(str2));	
}

int		lem_get_map(t_game *game, int flag)
{
	int		val;
	char	*output;

	game->done = 0;
	while ((val = get_next_line(0, &output)))
	{
		game->map = ft_joinstr(game->map, output);
		game->map = ft_joinstr(game->map, "\n");
		if (check_hash(output))
			map_check_hash(&output, &flag);
		else if (lem_valid_room(output) && !game->done)
		{
			game->room_data = ft_lem_push(game->room_data, build_rooms(output, flag));
			if (!valid_check(game->room_data, output))
				break ;
			flag = 3;
		}
		else if (lem_valid_path(game->room_data, output) && (game->done = 1))
			game->path_data = ft_lem_push(game->path_data, build_paths(output));
		else
			break ;
		ft_memdel((void **)&output);
	}
	ft_memdel((void **)&output);
	return (game->done == 1 ? val : 1);
}
