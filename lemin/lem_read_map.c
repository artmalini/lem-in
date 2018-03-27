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

int		lem_get_ants(void)
{
	int		i;
	int		val;
	char	*output;

	i = -1;
	if (get_next_line(0, &output) != 1)
		lem_error();
	while (ft_isdigit(output[++i]));
	if (output[i] != '\0')
		lem_error();
	val = ft_atoi(output);
	ft_memdel((void**)&output);
	return (val);
}

t_room		*build_rooms(char *output, int status)
{
	char	**rooms;
	t_room	*param;

	if (!(param = (t_room *)malloc(sizeof(t_room))))
		lem_error();
	rooms = ft_strsplit(output, ' ');
	param->name = ft_strdup(rooms[0]);
	param->x = ft_atoi(rooms[1]);
	param->y = ft_atoi(rooms[2]);
	param->flag = status;
	param->full = 0;
	param->ant = 0;
	param->paths = NULL;
	lem_split_free(rooms);
	return (param);
}

t_links		*build_paths(char *out)
{
	char	**path;
	t_links	*road;

	if (!(road = (t_links *)malloc(sizeof(t_links))))
		lem_error();
	path = ft_strsplit(out, '-');
	road->link1 = ft_strdup(path[0]);
	road->link2 = ft_strdup(path[1]);
	lem_split_free(path);
	return (road);
}

/*int			lem_coord_links(char *out)
{
	int		y;
	int		x;
	int		val;
	char	**path;

	val = 1;
	path = ft_strsplit(out, ' ');
	x = ft_atoi(path[1]);
	y = ft_atoi(path[2]);
	if (x == y)
		val = 0;
	lem_split_free(path);
	return (val);
}*/

int			lem_valid_room(char *out)
{
	char	*str;

	str = out;
	if (out[0] == 'L' || out[0] == '#')
		return (0);
	while (lem_symbols(*out) && *out != '-')
		out++;
	if (*out == 32)
		out++;
	else
		return (0);
	while (lem_symbols(*out))
		out++;
	if (*out == 32)
		out++;
	else
		return (0);
	while (lem_symbols(*out))
		out++;
	if (*out != '\0')
		return (0);
	return (1);
}

int			valid_path_cast(char *output)
{
	int		i;
	int		k;

	i = -1;
	k = 0;
	if (output[0] == 'L' || output[0] == '#')
		return (0);
	if (!lem_symbols(*output))
		return (0);
	while (lem_symbols(output[++i]) && output[i] != '-');
	if (output[i] == '-')
	{
		k++;
		i++;
	}
	else
		return (0);
	if (!lem_symbols(output[i]) && k != 1)
		return (0);
	i -= 1;
	while (lem_symbols(output[++i]) && output[i] != '-');
	if (output[i] != '\0')
		return (0);
	return (1);
}

t_room		*room_name(t_data *rooms, char *nbr)
{
	int		val;
	t_room	*room;

	val = 0;
	while (rooms && !val)
	{
		room = (t_room *)rooms->data;
		val = !ft_strcmp(nbr, room->name);
		rooms = rooms->next;
	}
	return (val ? room : 0);
}

int			lem_valid_path(t_data *rooms, char *output)
{	
	int		val;
	char	**path;
	t_links nbr;

	val = 0;
	if (!valid_path_cast(output))
		return (0);
	path = ft_strsplit(output, '-');
	nbr.link1 = ft_strdup(path[0]);
	nbr.link2 = ft_strdup(path[1]);
	if (room_name(rooms, nbr.link1) && room_name(rooms, nbr.link2))
		val = 1;
	lem_split_free(path);
	ft_strdel(&nbr.link1);
	ft_strdel(&nbr.link2);
	return (val);
}

int		valid_check(t_data *map, char *output)
{
	char	**str;
	char	*name;
	int		xx;
	int		yy;

	str = ft_strsplit(output, ' ');
	name = ft_strdup(str[0]);
	xx = ft_atoi(str[1]);
	yy = ft_atoi(str[2]);
	while (map->next)
	{
		if ((xx == ((t_room *)map->data)[0].x && yy ==((t_room *)map->data)[0].y)
			|| !ft_strcmp(name, ((t_room *)map->data)[0].name))
		{
			ft_strdel(&name);
		 	return (0);	
		}
		map = map->next;
	}
	lem_split_free(str);
	ft_strdel(&name);
	return (1);
}

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
		//printf("str1 %s str2 %s\n", str1, str2);
		if (!(str3 = (char *)malloc(sizeof(str3) * ((len1 + len2) + 1))))
			return (NULL);
		*str3 = 0;
		str3 = ft_strcat(ft_strcat(str3, str1), str2);
		return (str3);
	}
	else
		return (ft_strdup(str2));
	//printf("STR3 %s\n", str3);
	//str3 = ft_strcat(str3, ft_strdup("\n"));
	//free(str1);
	//free(str2);	
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
