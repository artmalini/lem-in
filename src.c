#include <stdio.h>
#include <fcntl.h>
# include "libft/libft.h"

# define USAGE_ERR 1
# define TO_MANY_ARGS 2
# define INVALID_MAP 3
# define NO_SOLUTION 4
# define MEMMORY 5

typedef	struct		s_data
{
	void			*data;
	struct s_data	*next;
}					t_data;

typedef struct		s_links
{
	char			*link1;
	char			*link2;
}					t_links;

typedef struct			s_room
{
	char				*name;
	int					x;
	int					y;
	int					flag;
	int					full;
	int					ant;
	t_data				*paths;
}						t_room;

typedef struct			s_ant
{
	int					id;
	t_room				*room;
	t_room				*last;
	int					ant_turn;
}						t_ant;

typedef struct			s_game
{
	int					total;
	int					done;
	//long int			moves;
	t_ant				*ant_list;
	t_data				*room_list;
	t_data				*path_list;
}						t_game;

void	lem_lst_foreach(t_data *lst, void (*f)());
void	lem_lst_data_free(t_data *first);


int		ft_isprint1(int c)
{
	if (c > 32 && c < 127)
		return (1);
	return (0);
}

static void		lem_split_free(char **str)
{
	int		i;

	i = -1;
	while (str[++i])
		ft_memdel((void**)&str[i]);
	ft_memdel((void**)&str);
}

/*static int		ft_isspace(char c)
{
	if (c == 32 || c == '\n' || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}*/

static void		lem_error(int error)
{
	if (error == USAGE_ERR)
		ft_putstr("usage: ./lem-in < map");
	else if (error == TO_MANY_ARGS)
		ft_putstr("error: to many arguments");
	else if (error == INVALID_MAP)
		ft_putstr("error: invalid map");
	else if (error == NO_SOLUTION)
		ft_putstr("error: the map doesn't have any solutions");
	else if (error == MEMMORY)
		ft_putstr("error: memmory allocation failed");
	ft_putchar('\n');
	exit(1);
}

static void		param_error(char *arg, char *argv)
{
	ft_putstr("error: Argument ");
	ft_putstr(arg);
	ft_putstr(" is invalid!");
	ft_putstr(" Try run ");
	ft_putstr(argv);
	ft_putstr(" < map\n!");
	exit(1);
}

t_data	*ft_lem_push(t_data *map, void *param)
{
	t_data	*res;
	t_data	*tmp;

	if (!param)
		lem_error(3);
	if (!(res = (t_data *)malloc(sizeof(t_data))))
		lem_error(5);
	if (!map)
	{
		//printf("one\n");
		res->data = param;
		res->next = NULL;
		map = res;
	}
	else
	{
		tmp = map;
		while (tmp->next)
			tmp = tmp->next;
		//printf("two\n");
		res->data = param;
		res->next = NULL;
		tmp->next = res;
	}
	return (map);
}
/*t_data	*ft_lem_push(t_data *first, void *item)
{
	t_data	*tmp;

	if (!item)
		return (NULL);
	if (!first)
	{
		first = (t_data *)malloc(sizeof(t_data));
		first->data = item;
		first->next = NULL;
	}
	else
	{
		tmp = first;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = (t_data *)malloc(sizeof(t_data));
		tmp->next->data = item;
		tmp->next->next = NULL;
	}
	return (first);
}*/



static int		lem_get_ants(void)
{
	int		i;
	int		val;
	char	*output;

	i = -1;
	if (get_next_line(0, &output) != 1)
		lem_error(3);
	//printf("@|%s|\n", output);
	
	while (ft_isdigit(output[++i]));
	//i -= 1;
	//while (ft_isspace(output[++i]));
	//printf("@|%c|\n", output[i]);
	if (output[i] != '\0')
		lem_error(3);
	val = ft_atoi(output);
	//printf("@|%s|\n", output);
	ft_memdel((void**)&output);
	return (val);
}

static int		check_hash(char *output)
{
	if (*output != '#')
		return (0);
	else if (ft_strequ("##start", output))
		return (1);
	else if (ft_strequ("##end", output))
		return (3);
	else
		return (2);
}

t_room		*build_rooms(char *output, int status)
{
	char	**rooms;
	t_room	*param;

	if (!(param = (t_room *)malloc(sizeof(t_room))))
		lem_error(5);
	rooms = ft_strsplit(output, ' ');
	param->name = ft_strdup(rooms[0]);/////////////////LEAK
	param->x = ft_atoi(rooms[1]);
	param->y = ft_atoi(rooms[2]);
	param->flag = status;
	param->full = 0;
	param->ant = 0;
	param->paths = NULL;
	lem_split_free(rooms);
	printf("room->name |%s|\n", param->name);
	printf("rooms|%s| status|%d| param->x|%d| param->y|%d|\n", param->name, status, param->x, param->y);

	return (param);
}

t_links		*build_paths(char *out)
{
	char	**path;
	//int		i;
	t_links	*road;

	//i = -1;
	if (!(road = (t_links *)malloc(sizeof(t_links))))
		lem_error(5);
	printf("build_paths\n");
	path = ft_strsplit(out, '-');
	road->link1 = ft_strdup(path[0]);
	road->link2 = ft_strdup(path[1]);
	//road->link1 = ft_strsub(out, 0, ft_int_length(ft_atoi(out)));/////////////////LEAK
	//road->link2 = ft_strdup(ft_strchr(out, '-') + 1);/////////////////LEAK
	//path = ft_strsplit(out, '');
	//lem_split_free(path);
	lem_split_free(path);
	printf("link1|%s| link2|%s|\n", road->link1, road->link2);
	return (road);
}

/*int			count_char(char *out, char c)
{
	int		val;

	val = 0;
	while (*out)
	{
		if (*out == c)
			val++;
		out++;
	}
	return (val);
}*/

static int			lem_valid_room(char *out)
{
	printf("valid |%s|\n", out);
	if (out[0] == 'L' || out[0] == '#')
	{
		printf("no\n");
		return (0);
	}
	while (ft_isprint1(*out) && *out != '-')
		out++;
	//else
	//	return (0);
	if (*out == 32)
		out++;
	else
		return (0);
	while (ft_isprint1(*out))
		out++;
	if (*out == 32)
		out++;
	else
		return (0);
	while (ft_isprint1(*out))
		out++;
	if (*out != '\0')
		return (0);
	//printf("!!!!!!!!!!!!!!!room valid |%s|\n", out);
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
		//printf("room room->name %s\n", room->name);
		rooms = rooms->next;
	}
	//if (room && room->name)
	//	printf("room room->name %s val|%d\n", room->name, val);
	return (val ? room : 0);
}

t_room		*place_to_chamber(t_data *rooms, int direct)
{
	int		val;
	t_room	*room;

	val = 0;
	while (rooms && !val)
	{
		room = (t_room *)rooms->data;
		val = room->flag == direct ? 1 : 0;
		//printf("place_to_chamber %s val|%d\n", room->name, val);
		rooms = rooms->next;
	}
	//if (room)
	//	printf("place_to_chamber %s val|%d\n", room->name, val);
	return (val ? room : 0);
}

static int			valid_path_cast(char *output)
{
	int		i;
	int		k;

	i = -1;
	k = 0;
	if (output[0] == 'L' || output[0] == '#')
	{
		printf("@@no\n");
		return (0);
	}
	if (!ft_isprint1(*output))
	{
		printf("err valid_path_casts\n");
		return (0);
	}
	while (ft_isprint1(output[++i]) && output[i] != '-');
		//printf("!|%s|\n", output[i]);
	if (output[i] == '-')
	{
		k++;
		i++;
	}
	else
	{
		printf("valid no1\n");
		return (0);
	}
	if (!ft_isprint1(output[i]) && k != 1)
	{
		printf("valid no2\n");
		return (0);
	}
	i -= 1;
	while (ft_isprint1(output[++i]) && output[i] != '-');
		//printf("!|%s|\n", output[i]);
	if (output[i] != '\0')
	{
		printf("last err valid_path_casts\n");
		return (0);
	}
	return (1);
}

static int			lem_valid_path(t_data *rooms, char *output)
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
	//nbr.link1 = ft_strsub(output, 0, ft_int_length(ft_atoi(output)));
	//nbr.link2 = ft_strdup(ft_strchr(output, '-') + 1);
	printf("lem_valid_path ##%s %s\n", nbr.link1, nbr.link2);
	if (room_name(rooms, nbr.link1) && room_name(rooms, nbr.link2))
	{
		val = 1;
		printf("ok\n");
	}
	lem_split_free(path);
	ft_strdel(&nbr.link1);
	ft_strdel(&nbr.link2);
	//free(nbr.link1);
	//free(nbr.link2);
	//nbr.link1 = NULL;
	//nbr.link2 = NULL;
	return (val);
}

int		check(t_data *map, char *output)
{
	char	**str;	
	char	*name;
	int		xx;
	int		yy;

	str = ft_strsplit(output, ' ');
	name = ft_strdup(str[0]);
	xx = ft_atoi(str[1]);
	yy = ft_atoi(str[2]);
	//printf("x %d y %d\n", xx, yy);
	while (map->next)
	{
		if ((xx == ((t_room *)map->data)[0].x && yy == ((t_room *)map->data)[0].y) || !ft_strcmp(name, ((t_room *)map->data)[0].name))
		{
			printf("err yep\n");
			ft_strdel(&name);
		 	return (0);		
		}
		//printf("view map->x|%d| map->y|%d|\n", ((t_room *)map->data)[0].x, ((t_room *)map->data)[0].y);
		map = map->next;
	}
	lem_split_free(str);
	ft_strdel(&name);
	return (1);
}

void		map_check_hash(char **output, int *status)
{
	//int	*p;

	//p = status;
	if (*status == 2)
	{
		*status = check_hash(*output);
		//ft_memdel((void **)&(*output));
		//printf("$$$$%d\n", *status);
	}
	else
	{
		*status = *status;
		//printf("***%d\n", *status);
	}
}

static int		lem_get_map(t_game *game)
{
	int		val;
	int		status;
	char	*output;

	status = 2;
	game->done = 0;
	while ((val = get_next_line(0, &output)))
	{		
		if (check_hash(output))
			map_check_hash(&output, &status);
		else if (lem_valid_room(output) && !game->done)
		{
			//build_room(output, status);
			game->room_list = ft_lem_push(game->room_list, build_rooms(output, status));
			if (!check(game->room_list, output))////@@@@@@@@@@@@@
				break ;
			status = 2;
		}
		else if (lem_valid_path(game->room_list, output) && (game->done = 1))
			game->path_list = ft_lem_push(game->path_list, build_paths(output));
		else
			break ;
		ft_memdel((void **)&output);
	}
	ft_memdel((void **)&output);
	printf("end map val %d\n", val);
	return (game->done == 1 ? val : 1);
}

void	check1(t_data *list, int room)
{
	//printf("list->content list->x|%d| list->y|%d|\n", list->x, list->y);
	while (list)
	{
		//printf("yep\n");
		printf("path->link1|%s| t_data *room->flag|%d|\n", ((t_links *)list->data)[0].link1, room);
		list = list->next;
	}
}

// void		lem_set_path1(t_game *game)
// {
// 	t_room	*room;
// 	t_links	*links;
// 	t_data	*tmp_room;
// 	t_data	*tmp_path;

// 	tmp_room = game->room_list;
// 	while (tmp_room != 0)
// 	{
// 		tmp_path = game->path_list;
// 		room = (t_room *)tmp_room->data;
// 		while (tmp_path != 0)
// 		{
// 			links = (t_links *)tmp_path->data;
// 			tmp_path = tmp_path->next;
// 		}
// 		tmp_room = tmp_room->next;
// 	}
// 	printf("!!!3\n");
// }

/*int		review(t_game *game, char *link1, char *link2, char *room_name)
{
	t_room	*room;
	t_links	*links;
	t_data	*tmp_room;
	t_data	*tmp_path;
	char	*tmp_link1;
	//t_data	*tpaths;

	tmp_room = game->room_list;
	while (tmp_room != 0)
	{
		tmp_path = game->path_list;
		room = (t_room *)tmp_room->data;
		while (tmp_path != 0)
		{
			links = (t_links *)tmp_path->data;
			if (!ft_strcmp(link1, room_name) && !ft_strcmp(links->link1, room->name))
			{
				//printf("ga\n");
				tmp_link1 = link1;
				link1 = link2;
				link2 = tmp_link1;
				review(game, link1, link2, room_name);
				//room->paths = ft_lem_push(room->paths, room_name(game->room_list, links->link2));
				printf("@@@@@@@@@@@@@@@%s\n", room->name);
				//check1(room->paths, room->flag);					
			}
			if (!ft_strcmp(link2, room_name) && !ft_strcmp(links->link2, room->name))
			{
				//printf("ga1\n");
				tmp_link1 = link2;
				link2 = link1;
				link1 = tmp_link1;
				review(game, link1, link2, room_name);
				//room->paths = ft_lem_push(room->paths, room_name(game->room_list, links->link1));
				printf("###############%s\n", room->name);
			}
			tmp_path = tmp_path->next;
		}
		tmp_room = tmp_room->next;
	}
	return (0);
}*/

int		search(t_data *tmp_room, char *link)
{
	int		i;
	t_room	*room;

	i = 0;
	while (tmp_room != 0 && !i)
	{
		room = (t_room *)tmp_room->data;
		if (!ft_strcmp(room->name, link))
			i = (room->flag == 3) ? 1 : 0;
		tmp_room = tmp_room->next;
	}
	return (i);
}

int		fin_lem_review(t_data *path, t_data *rooms, t_game *game, char *link1, int i)
{
	while (++i < 2)
	{
		while (rooms)
		{
			while (path)
			{
				if (!ft_strcmp(((t_links *)path->data)[0].link1, link1))
				{
					link1 = ((t_links *)path->data)[0].link2;
					if (search(game->room_list,
						((t_links *)path->data)[0].link2))
						return (1);
					break ;
				}
				path = path->next;
			}
			path = game->path_list;
			rooms = rooms->next;
		}
		rooms = game->room_list;
	}
	return (0);
}

int		lem_review(t_game *game, char *link1)
{
	t_data	*path;
	t_data	*rooms;

	rooms = game->room_list;
	path = game->path_list;
	if (fin_lem_review(path, rooms, game, link1, -1))
		return (1);
	return (0);
}

void		lem_set_path(t_game *game)
{
	t_room	*room;
	t_links	*links;
	t_data	*tmp_room;
	t_data	*tmp_path;

	tmp_room = game->room_list;
	while (tmp_room)
	{
		tmp_path = game->path_list;
		room = (t_room *)tmp_room->data;
		while (tmp_path)
		{
			links = (t_links *)tmp_path->data;
			if (!ft_strcmp(links->link1, room->name) &&
				lem_review(game, links->link1))
					room->paths = ft_lem_push(room->paths,
						room_name(game->room_list, links->link2));
			if (!ft_strcmp(links->link2, room->name) &&
				lem_review(game, links->link2))
					room->paths = ft_lem_push(room->paths,
						room_name(game->room_list, links->link1));
			tmp_path = tmp_path->next;
		}
		tmp_room = tmp_room->next;
	}
	printf("!!!2\n");
}

/*int		lem_review(t_data *tmp_room, t_data *tmp_path, char *link1, int i)
{
	//t_room	*room;
	t_links	*links;
	t_data	*path;
	t_data	*rooms;
	char	*tmp_link;

	rooms = tmp_room;
	path = tmp_path;
	tmp_link = link1;
	while (++i < 2)// && room->flag != 3
	{
		while (rooms != 0)
		{	
			//room = (t_room *)rooms->data;
			while (path != 0)
			{
				links = (t_links *)path->data;
				//printf("links->link1|%s| |%d|\n", links->link1, room->flag);
				if (!ft_strcmp(links->link1, tmp_link))
				{
					//printf("**********************|%s| flag|%d|", links->link1, room->flag);
					tmp_link = links->link2;
					if (search(tmp_room, tmp_link))
						return (1);
					//printf(" neighb_link |%s| |%d|\n", tmp_link, ((t_room *)rooms->data)[0].flag);
					break ;
				}
				path = path->next;
			}
			path = tmp_path;
			rooms = rooms->next;
		}
		rooms = tmp_room;
	}
	return (0);
}*/

/*void		lem_set_path(t_game *game)
{
	t_room	*room;
	t_links	*links;
	t_data	*tmp_room;
	t_data	*tmp_path;
	//t_data	*tpaths;

	tmp_room = game->room_list;
	while (tmp_room != 0)
	{
		tmp_path = game->path_list;
		room = (t_room *)tmp_room->data;
		while (tmp_path != 0)
		{
			links = (t_links *)tmp_path->data;
			if (!ft_strcmp(links->link1, room->name))
			{
				//printf("ga\n");
				if (lem_review(game->room_list, game->path_list, links->link1, -1))
					room->paths = ft_lem_push(room->paths, room_name(game->room_list, links->link2));
				//printf("@@@@@@@@@@@@@@@%s\n", room->name);
				//check1(room->paths, room->flag);					
			}
			if (!ft_strcmp(links->link2, room->name))
			{
				//printf("ga1\n");
				if (lem_review(game->room_list, game->path_list, links->link2, -1))
					room->paths = ft_lem_push(room->paths, room_name(game->room_list, links->link1));
				//printf("###############%s\n", room->name);
				//check1(room->paths, room->flag);
				//check1(room->paths);	
			}
			tmp_path = tmp_path->next;
		}
		tmp_room = tmp_room->next;
	}
	printf("!!!2\n");
}*/

t_ant		*lem_ant_struct(t_data *room, int ants)
{
	int		i;
	t_ant	*insects;

	i = 0;
	if (!(insects = (t_ant *)malloc(sizeof(t_ant) * ants)))
		lem_error(5);
	while (i < ants)
	{
		insects[i].id = i + 1;
		insects[i].ant_turn = 0;		
		insects[i].room = place_to_chamber(room, 1);
		i++;
	}
	return (insects);
}

// void	recalc(t_data *paths)
// {
// 	t_room	*room;

// 	while (paths)
// 	{
// 		room = (t_room *)paths->data;
// 		printf("$$$$$$$$$|%s|\n", room->name);
// 		paths = paths->next;
// 	}
// }

int		valid_rooms(t_data *lst)
{
	int		first;
	int		last;
	t_room	*room;

	first = 0;
	last = 0;
	while (lst)
	{
		room = (t_room *)lst->data;
		if (room->flag == 1)
			first++;
		if (room->flag == 3)
			last++;
		lst = lst->next;
	}
	//printf("first %d last %d\n", first, last);
	return (first == 1 && last == 1 ? 1 : 0);
}

static int		lem_read_map(t_game *game)
{
	game->total = lem_get_ants();//ants count
	//printf("game->total|%d|\n", game->total);	
	if (game->total == 0 || lem_get_map(game))
		return (0);
	if (!valid_rooms(game->room_list))
		return (0);
	//recalc(game->path_list);
	game->ant_list = lem_ant_struct(game->room_list, game->total);//set ants to start	
	//printf("ttttt\n");
	return (1);
}

static void		lem_struct(t_game *game)
{
	game->total = 0;
	game->done = 0;
	//game->moves = 0;
	game->ant_list = NULL;
	game->room_list = NULL;
	game->path_list = NULL;
	//return (lem);
}

static int		lem_last_ant(t_ant *ants, int numbrs)
{
	int		i;

	i = -1;
	while (++i < numbrs && ants != 0)
	{
		if (ants[i].room->flag != 3)
			return (0);
	}
	return (1);
}

int		lem_last_ant1(t_ant *ants, int numbrs)
{
	int		i;

	i = -1;
	while (++i < numbrs && ants != 0)
	{
		if (ants[i].room->flag != 3)
			return (ants[i].id);
	}
	printf("all right ");
	return (0);
}

static int		ants_ready(t_ant *ants, int numbrs)
{
	int		i;

	i = -1;
	while (++i < numbrs)
		ants[i].ant_turn = 0;
	return (1);
}

//static void		role_ant(t_game *game)
//{}

static void	print_ant(t_ant *ant)
{
	ft_putchar('L');
	ft_putnbr(ant->id);
	ft_putchar('-');
	ft_putstr(ant->room->name);
	ft_putchar(' ');
	return ;
}


static void	move_ants(t_ant *ant, t_room *room)
{
	ant->room->ant = 0;
	ant->last = ant->room;
	//printf("ant->room %s\n",((t_room *)ant)[0].room);
	ant->room = room;
	ant->room->ant = 1;
	print_ant(ant);
	//game->moves += 1;///not important
	return ;
}

int			find_room(void *room, int last)
{
	t_room	*current;
	t_data	*testing;
	int		find;
	int		old_find;

	current = (t_room *)room;
	if (current->flag == last)
		return (0);
	if (current->full)
		return (-1);
	//printf("find_room current->flag %d\n", current->flag);
	current->full = 1;
	find = 2147483647;
	testing = current->paths;
	//printf("testing %s\n", ((t_links *)testing->data)[0].link1);
	//printf("go ahead\n");
	while (testing)
	{
		//printf(".link1 %s\n", ((t_links *)testing->data)[0].link1);
		if ((old_find = find_room(testing->data, last)) < find && old_find != -1)
		{
			//printf("old_find %d %s\n", old_find, ((t_links *)testing->data)[0].link1);
			find = 1 + old_find;
		}
		testing = testing->next;
	}
	current->full = 0;
//	printf("find %d\n", find);
	return (find == 2147483647 ? -1 : find);
}

void		lem_play(t_game *game)
{
	int		result;
	int		distance;
	t_data	*ls;
	t_room	*tmp;
	t_room	*next;

	distance = 2147483647;
	ls = game->ant_list->room->paths;
	while (ls)
	{
		tmp = (t_room *)ls->data;
		if ((tmp->flag == 3 || !tmp->ant) && tmp != game->ant_list->last)
		{
			//printf("go\n");
			result = find_room(tmp, 3);
			if (result < distance && result > -1)
			{
				//printf("newpath %d\n", result);
				distance = result;
				next = tmp;
			}
		}
		ls = ls->next;
	}
	if (distance < 2147483647)
		move_ants(game->ant_list, next);
}

	// t_room	*room;
	// t_links	*links;
	// t_data	*tmp_room;
	// t_data	*tmp_path;

	// tmp_room = game->room_list;
	// while (tmp_room != 0)
	// {
	// 	tmp_path = game->path_list;
	// 	room = (t_room *)tmp_room->data;

void	lem_lst_foreach(t_data *lst, void (*f)())
{
	t_data		*tmp;

	tmp = lst;
	while (tmp)
	{
		(*f)(tmp->data);
		tmp = tmp->next;
	}
}

void	lem_lst_data_free(t_data *first)
{
	//t_room	*room;
	t_data	*tmp;

	tmp = first;
	//room = (t_room *)first;
	while (first)
	{
		//lem_lst_data_free(first->next);
		//room = (t_room *)first;
		//ft_strdel(&room->name);
		tmp = first;		
		//first = NULL;
		first = first->next;
		free(tmp);
	}
}

// void	ft_lst_rec(t_data *first)
// {
// 	t_room	*room;

	
	
// 	if (first)
// 	{
// 		room = (t_room *)first;
// 		printf("@@room->name|%s|\n", room->name);	
// 		ft_lst_rec(first->next);
// 		ft_strdel(&room->name);
// 		//free(first);
// 	}
// }

static void	delete_rooms(void *data)
{
	t_room	*room;
	//int i = -1;
	room = (t_room *)data;
	//printf("~@@room->name|%s|\n", room->name);
	//while (++i < 3)
	ft_strdel(&room->name);
	//printf("~@@room->name|%s|\n", room->name);
	lem_lst_data_free(room->paths);
	//ft_strdel(&room->name);
	free(data);
	data = NULL;
	return ;
}

static void	delete_paths(void *data)
{
	t_links	*path;

	path = (t_links *)data;
	ft_strdel(&path->link1);
	ft_strdel(&path->link2);
	free(data);
	data = NULL;
	return ;
}


static int		check_turn(t_ant *ants)
{
	t_room	*path;
	t_data	*list;

	if (ants->room->flag == 3)
		return (0);
	list = ants->room->paths;
	//printf("ants->room->ant %d\n", ants->room->ant);
	while (list && !ants->ant_turn)
	{
		path = (t_room *)list->data;
		//printf("$$$path->ant %d path->ant %d\n", path->ant, path->ant);
		if (path->flag == 3 || (!path->ant && path !=
			ants->last && path->flag != 1))
		{	
			//printf("pk\n");	
			return (1);
		}
		list = list->next;
	}
	//printf("rrrrrrrrrrrrrrrrr\n");
	return (0);
}

static void		lem_player(t_game *game)
{
	int		i;
	int		turn;

	i = -1;
	turn = 1;
	//while (!lem_last_ant(game->ant_list, game->total))
	//{
		ants_ready(game->ant_list, game->total);
		//printf("ss\n");
		//lem_play(game);
		//role_ant(game);
		while (turn && !lem_last_ant(game->ant_list, game->total))
		{
			i = -1;
			turn = 0;
			while (++i < game->total)
			{
				//printf("nni %d\n", i);
				if (check_turn(game->ant_list + i))
				{
					turn = 1;
					game->ant_list += i;
					//printf("@@@yei %d\n", i);
					lem_play(game);
					game->ant_list -= i;
				}
			}
			ft_putchar('\n');
		}
		turn = 1;
		//reset_rooms(game->ant_list, game->total);
		//while (!lem_last_ant(game->ant_list, game->total))
		//{}
		printf("lem las %d\n", lem_last_ant1(game->ant_list, game->total));		
		if (lem_last_ant1(game->ant_list, game->total))//SHOULD be ERROR when ret ant.[id] == 1
			lem_error(3);		 
		printf("player game->total|%d| i|%d|\n", game->total, i);
	//}
}

void		lem_free(t_game *game)
{
	//t_room *room;
	//t_data *tmp_room;

	if (game)
	{
		/*if(game)
		{
			//delete_rooms1(game->ant_list->room->paths);
			delete_rooms1(game);

			//lem_lst_foreach(game->ant_list->room->paths, delete_rooms1);
			//lem_lst_data_free(game->ant_list->room->paths);
		}*/
		//if (game->room_list)
		/*if (game->ant_list->room->paths)
		{
			t_data	*ls;
			t_room	*tmp;
			//t_data	*rw;


			ls = game->ant_list->room->paths;
			while (ls)
			{
				tmp = (t_room *)ls->data;
				printf("##################tmp->flag|%d|\n", tmp->ant);
				//rw =ls;				
				ls = ls->next;
				//free(rw);
			}
		}*/		
		if (game->room_list)
		{
			//tmp_room = game->room_list;
			//room = (t_room *)tmp_room->data;
			//lem_lst_foreach(room, delete_rooms);
			lem_lst_foreach(game->room_list, delete_rooms);
			lem_lst_data_free(game->room_list);
		}
		if (game->path_list)
		{
			lem_lst_foreach(game->path_list, delete_paths);
			lem_lst_data_free(game->path_list);
		}
		if (game->ant_list)
			ft_memdel((void **)&game->ant_list);
		ft_memdel((void **)&game);
	}
}

static void		lem_parse(int argc, char **argv)
{
	t_game	*game;

	while (--argc)
		param_error(argv[1], argv[0]);
	if(!(game = (t_game *)malloc(sizeof(t_game))))
		lem_error(5);
	lem_struct(game);
	if (!lem_read_map(game))
	{
		lem_free(game);
		lem_error(3);
	}

	lem_set_path(game); ///////////
	//lem_valid_room2();//// first end last room IMPORTANT
	lem_player(game);

	lem_free(game);
	//ft_memdel((void**)&game);
}

int				main(int argc, char **argv)
{
	//int		fd;

	//printf("%d\n", argc);
	//if (argc > 1)
	//	lem_error(2);
	if (argc < 1)
		lem_error(1);
	//else if(!(fd = open(argv[0], O_RDONLY)))
	//	lem_error(3);
	else
		lem_parse(argc, argv);

	//system("leaks lem-in");
	//close(fd);	
	return (0);
}
