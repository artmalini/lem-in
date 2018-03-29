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
	if (val == -2147483648 || val <= 0)
		lem_error();
	ft_memdel((void**)&output);
	return (val);
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

t_data		*ft_lem_push(t_data *map, void *param)
{
	t_data	*res;
	t_data	*tmp;

	if (!param)
		lem_error();
	if (!(res = (t_data *)malloc(sizeof(t_data))))
		lem_error();
	if (!map)
	{
		res->data = param;
		res->next = NULL;
		map = res;
	}
	else
	{
		tmp = map;
		while (tmp->next)
			tmp = tmp->next;
		res->data = param;
		res->next = NULL;
		tmp->next = res;
	}
	return (map);
}
