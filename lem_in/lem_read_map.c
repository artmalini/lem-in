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

int			check_hash(char *output)
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

char		*ft_joinstr(char *str1, char *str2)
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
		free(str1);
		return (str3);
	}
	else
	{
		str3 = ft_strdup(str2);
		return (str3);
	}
}

int			lem_get_map(t_game *game, int fl, int done, int val)
{
	char	*out;
	char	*tmp;

	while ((val = get_next_line(0, &out)))
	{
		tmp = ft_joinstr(game->map, out);
		game->map = ft_strjoin(tmp, "\n");
		ft_strdel(&tmp);
		if (check_hash(out))
			map_check_hash(&out, &fl);
		else if (lem_valid_room(out) && !done)
		{
			game->room_data = lem_push(game->room_data, build_rooms(out, fl));
			if (!valid_check(game->room_data, out))
				break ;
			fl = 3;
		}
		else if (lem_valid_path(game->room_data, out) && (done = 1))
			game->path_data = lem_push(game->path_data, build_paths(out));
		else
			break ;
		ft_memdel((void **)&out);
	}
	ft_memdel((void **)&out);
	return (val);
}
