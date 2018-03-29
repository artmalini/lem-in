#include "includes/lem_in.h"

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
	while (ft_isdigit(*out))
		out++;
	if (*out == 32)
		out++;
	else
		return (0);
	while (ft_isdigit(*out))
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
