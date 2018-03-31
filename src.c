#include <stdio.h>
#include <fcntl.h>
# include "libft/libft.h"

# define USAGE_ERR 1
# define TO_MANY_ARGS 2
# define INVALID_MAP 3
# define NO_SOLUTION 4
# define MEMMORY 5

# define SWARM 100000
//# define color(x) ({"\033[40", "\033[41"};)

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
	int					ant_turn;
	t_room				*room;
	t_room				*last;
}						t_ant;

typedef struct			s_game
{
	int					total;
	int					done;
	int					visual;
	int					map_hide;
	//long int			moves;
	t_ant				*ant_data;
	t_data				*room_data;
	t_data				*path_data;
	//char				*best_data;
	char				*map;
}						t_game;

void	lem_lst_foreach(t_data *lst, void (*f)());
void	lem_lst_data_free(t_data *first);


int		lem_symbols(int c)
{
	if (c > 32 && c < 127)
		return (1);
	return (0);
}

void		lem_split_free(char **str)
{
	int		i;

	i = -1;
	while (str[++i])
		ft_memdel((void**)&str[i]);
	ft_memdel((void**)&str);
}

/*int		ft_isspace(char c)
{
	if (c == 32 || c == '\n' || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}*/

void		lem_error(int error)
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

void		print_header_ant(void)
{
	ft_putstr("                .-              -`                \n");
    ft_putstr("               `-                -`               \n");
    ft_putstr("               -                  -               \n");
    ft_putstr("              .`                  ..              \n");
    ft_putstr("              -                    -              \n");
    ft_putstr("              -       .syys.       -              \n");
    ft_putstr("              :://::-:dddddd:-:://::              \n");
    ft_putstr("                    :dhhhhhhd:                    \n");
    ft_putstr("                    oddhhhdddo                    \n");
    ft_putstr("           .``..`   +dhhddddm+   `..``.           \n");
    ft_putstr("                -o  `+yddddy+` `o.                \n");
    ft_putstr("                 -/:`.hyhhdd.`:/.                 \n");
    ft_putstr("             `:::+//yoddhdddoy//+:::`             \n");
    ft_putstr("           ./:`   /y+-:dhdd:-+y:   `//.           \n");
    ft_putstr("         ..`        .:sdhdds:.        ..`         \n");
    ft_putstr("     .```      -/++o++//hd+/++o++/-      ```.     \n");
    ft_putstr("               +     .oyddyo.     +               \n");
    ft_putstr("               o`   :dhoydddd:   .+               \n");
    ft_putstr("               /-  .dhyyhhhddm.  :/               \n");
    ft_putstr("               --  odhsshddddmo  :-               \n");
    ft_putstr("              `-   sdhssyhdddds   -`              \n");
    ft_putstr("              :    +dhhyhddddm+    :              \n");
    ft_putstr("             -`    `yddhddddmy`    `-             \n");
    ft_putstr("           .-        oddddddo        -.           \n");
    ft_putstr("          .`          `+hh+`          `.          \n\n");
}

void		display_header_ant(int hide)
{
	if (hide == 1)
		ft_putchar('\n');
	ft_putstr("\033[31m");
	print_header_ant();
	ft_putstr("\033[0m\n");
}

void		display_header(void)
{
	ft_putstr("\033[31m");	
	ft_putstr("                             #\n");     
	ft_putstr(" ###                         ##\n");     
	ft_putstr(" ###    ######## ##   ## ### ###  ##\n");
	ft_putstr(" ###             ### ### ### #### ##\n");
	ft_putstr(" ###     ####### ####### ### #######\n");
	ft_putstr(" ###     ###     ## # ## ### ### ###\n");
	ft_putstr(" ####### ####### ##   ## ### ###  ##\n");
	ft_putstr("                 ##                #\n");
	ft_putstr("\033[0m\n");
}

void		param_error(char *arg, char *argv)
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
		ft_putstr(" < map_name\033[0m   To run without arguments\n");
	exit(1);
}

t_data	*lem_push(t_data *map, void *param)
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
/*t_data	*lem_push(t_data *first, void *item)
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



int		lem_get_ants(void)
{
	int		i;
	int		val;
	char	*output;

	i = -1;
	if (get_next_line(0, &output) != 1)
		lem_error(3);
	printf("@|%s|\n", output);
	
	while (ft_isdigit(output[++i]));
	//i -= 1;
	//while (ft_isspace(output[++i]));
	//printf("@|%c|\n", output[i]);
	if (output[i] != '\0')
		lem_error(3);
	val = ft_atoi(output);
	if (val == -2147483648 || val <= 0)
		lem_error(3);
	printf("GET ANTS @|%d|\n", val);
	ft_memdel((void**)&output);
	return (val);
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
	{
		printf("OUT |%s|\n", output);
		return (3);
	}
}

t_room		*build_rooms(char *output, int flag)
{
	char	**rooms;
	t_room	*param;

	if (!(param = (t_room *)malloc(sizeof(t_room))))
		lem_error(5);
	rooms = ft_strsplit(output, ' ');
	param->name = ft_strdup(rooms[0]);/////////////////LEAK
	param->x = ft_atoi(rooms[1]);
	param->y = ft_atoi(rooms[2]);
	param->flag = flag;
	param->full = 0;
	param->ant = 0;
	param->paths = NULL;
	lem_split_free(rooms);
	printf("room->name |%s|\n", param->name);
	printf("rooms|%s| flag|%d| param->x|%d| param->y|%d|\n", param->name, flag, param->x, param->y);

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
	//char	*str;

	//str = out;
	//printf("valid |%s|\n", out);
	if (out[0] == 'L' || out[0] == '#')
	{
		printf("no\n");
		return (0);
	}
	while (lem_symbols(*out) && *out != '-')
		out++;
	//else
	//	return (0);
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
	//if (*out != '\0' || !lem_coord_links(str))
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

int			valid_path_cast(char *output)
{
	int		i;
	int		k;

	i = 0;
	k = 0;
	if (output[0] == 'L' || output[0] == '#')
		return (0);
	if (!lem_symbols(*output))
		return (0);
	while (lem_symbols(output[i]) && output[i] != '-')
		i++;
	while (output[i] == '-')
	{
		k++;
		i++;
	}
	if (!lem_symbols(output[i]) && k != 1)
		return (0);
	while (lem_symbols(output[i]) && output[i] != '-')
		i++;
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

void		map_check_hash(char **output, int *flag)
{
	//int	*p;

	//p = flag;
	if (*flag == 3)
	{
		*flag = check_hash(*output);
		//ft_memdel((void **)&(*output));
		//printf("$$$$%d\n", *flag);
	}
	else
	{
		*flag = *flag;
		//printf("***%d\n", *status);
	}
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
		free(str1);
		return (str3);
	}
	else
	{
		str3 = ft_strdup(str2);
		return (str3);
	}	
}

// char	*ft_joinstr(char *str, char *n)
// {
// 	char	*out;

// 	out = ft_strjoin(str, n);
// 	printf("OUT %s\n", out);
// 	return (out);
// }

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
	printf("get_map |%d|\n", val);
	return (val);
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

// 	tmp_room = game->room_data;
// 	while (tmp_room != 0)
// 	{
// 		tmp_path = game->path_data;
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

	tmp_room = game->room_data;
	while (tmp_room != 0)
	{
		tmp_path = game->path_data;
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
				//room->paths = lem_push(room->paths, room_name(game->room_data, links->link2));
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
				//room->paths = lem_push(room->paths, room_name(game->room_data, links->link1));
				printf("###############%s\n", room->name);
			}
			tmp_path = tmp_path->next;
		}
		tmp_room = tmp_room->next;
	}
	return (0);
}*/

/*int		best_path(t_data *tmp_room, char *link)
{
	int		i;
	t_room	*room;

	i = 0;
	while (tmp_room != 0 && !i)
	{
		room = (t_room *)tmp_room->data;
		if (!ft_strcmp(room->name, link))
			i = (room->flag == 2) ? 1 : 0;
		tmp_room = tmp_room->next;
	}
	return (i);
}*/

int		fin_lem_review2(t_data *path, char *link1)
{
	while (path)
	{
		if ((!ft_strcmp(((t_links *)path->data)[0].link1, link1)))
			return (1);
		path = path->next;
	}
	return (0);
}

int		lem_review2(t_game *game, char *link1, int flag)
{
	t_data	*path;

	path = game->path_data;
	if (flag == 1 || flag == 2)
		return (1);
	else if (fin_lem_review2(path, link1))
		return (1);
	return (0);
}

int		fin_lem_review1(t_data *path, char *link1)
{
	while (path)
	{
		if ((!ft_strcmp(((t_links *)path->data)[0].link2, link1)))
			return (1);
		path = path->next;
	}
	return (0);
}

int		lem_review1(t_game *game, char *link1, int flag)
{
	t_data	*path;

	path = game->path_data;
	if (flag == 1 || flag == 2)
		return (1);
	else if (fin_lem_review1(path, link1))
		return (1);
	return (0);
}

/*int		lem_path_len(t_data *path)
{
	int		val;

	val = 0;
	while (path)
	{
		val++;
		path = path->next;
	}
	return (val);
}*/

void		lem_set_path(t_game *game)
{
	t_room	*room;
	t_links	*links;
	t_data	*tmp_room;
	t_data	*tmp_path;

	tmp_room = game->room_data;
	//int	len = lem_path_len(game->path_data);
	//printf("len %d\n", len);
	//printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@lem_path_len %d\n", lem_path_len(game->path_data));
	while (tmp_room)
	{
		tmp_path = game->path_data;
		room = (t_room *)tmp_room->data;
		while (tmp_path)
		{
			links = (t_links *)tmp_path->data;
			if (!ft_strcmp(links->link1, room->name) &&
				lem_review1(game, links->link2, room->flag))
			{
				//printf("@@@@@@@@@@@links->link2 %s |%d|\n", links->link2, room->flag);
					room->paths = lem_push(room->paths,
						room_name(game->room_data, links->link2));
			}
			if (!ft_strcmp(links->link2, room->name) &&
				lem_review2(game, links->link1, room->flag))
			{
				//printf("@@@@@@@@@@@links->link1 %s |%d|\n", links->link1, room->flag);
					room->paths = lem_push(room->paths,
						room_name(game->room_data, links->link1));
			}
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

	tmp_room = game->room_data;
	while (tmp_room != 0)
	{
		tmp_path = game->path_data;
		room = (t_room *)tmp_room->data;
		while (tmp_path != 0)
		{
			links = (t_links *)tmp_path->data;
			if (!ft_strcmp(links->link1, room->name))
			{
				//printf("ga\n");
				if (lem_review(game->room_data, game->path_data, links->link1, -1))
					room->paths = lem_push(room->paths, room_name(game->room_data, links->link2));
				//printf("@@@@@@@@@@@@@@@%s\n", room->name);
				//check1(room->paths, room->flag);					
			}
			if (!ft_strcmp(links->link2, room->name))
			{
				//printf("ga1\n");
				if (lem_review(game->room_data, game->path_data, links->link2, -1))
					room->paths = lem_push(room->paths, room_name(game->room_data, links->link1));
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
		if (room->flag == 2)
			last++;
		lst = lst->next;
	}
	printf("valid_rooms first %d last %d\n", first, last);
	return (first == 1 && last == 1 ? 1 : 0);
}

int		lem_read_map(t_game *game)
{
	game->total = lem_get_ants();//ants count
	//printf("game->total|%d|\n", game->total);	
	if (game->total == 0 || lem_get_map(game, 3, 0, 0))
		return (0);
	if (!valid_rooms(game->room_data))
		return (0);
	//recalc(game->path_data);
	printf("yep\n");
	//game->ant_data = l_ant_s(game->room_data, game->total);//set ants to start SLOW on many ants
	printf("*yep*\n");
	//printf("ttttt\n");

	// t_data *tmp;
	// tmp = game->map;
	// while (tmp)
	// {
	// 	printf("yyyyy\n");
	// 	printf("##### %s", (char *)tmp->data);
	// 	tmp = tmp->next;
	// }

	// char *tmp = game->map;
	// while (*tmp)
	// {
	// 	//printf("yyyyy\n");
	// 	printf("%c", *tmp);
	// 	tmp++;
	// }
	return (1);
}

void		lem_struct(t_game *game)
{
	game->total = 0;
	game->done = 0;
	//game->moves = 0;
	game->ant_data = NULL;
	game->room_data = NULL;
	game->path_data = NULL;
	game->map = NULL;
	game->visual = 0;
	game->map_hide = 0;
	//game->best_data = NULL;
	//return (lem);
}

int		lem_last_ant(t_ant *ants, int numbrs)
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

int		last_insects(t_ant *ants, int numbrs)
{
	int		i;

	i = -1;
	while (++i < numbrs && ants != 0)
	{
		if (ants[i].room->flag != 2)
			return (ants[i].id);
	}
	printf("all right ");
	return (0);
}



//void		role_ant(t_game *game)
//{}

char	*color(int x)
{
	//char	*rgb[] = {"\x1B[91m", "\x1B[92m", "\x1B[93m", "\x1B[94m", "\x1B[95m", "\x1B[96m"};
	char	*rgb[6];

	rgb[0] = "\x1B[91m";//red
	rgb[1] = "\x1B[92m";//green
	rgb[2] = "\x1B[93m";//yellow
	rgb[3] = "\x1B[94m";//Blue
	rgb[4] = "\x1B[95m";//Purple
	rgb[5] = "\x1B[96m";//Cyan
	return (rgb[x]);
}

void	print_ant(t_ant *ant, int vis)
{
	if (vis == 1)
	{
		//printf("ant->id %d |%d|\n", ant->id, ant->id % 6);
		//ft_putstr("\033");
		ft_putstr(color(ant->id % 6));
		//ft_putstr("Something\n");
		//ft_putstr("\033[31m");
	}
	ft_putchar('L');
	ft_putnbr(ant->id);
	ft_putchar('-');
	if (vis == 1 && ant->room->flag == 2)
	{
			ft_putstr("\x1B[90m");
	}
	ft_putstr(ant->room->name);
	ft_putchar(' ');
	if (vis == 1)
		ft_putstr("\033[0m");
	return ;
}


void	move_ants(t_ant *ant, t_room *room, int vis)
{
	//printf("before ant->room->ant %d\n", ant->room->ant);
	ant->room->ant = 0;
	ant->last = ant->room;
	//printf("after ant->last->ant %d\n", ant->last->ant);
	//printf("ANT ant->room %s\n",(char *)ant->room->name);
	ant->room = room;
	ant->room->ant = 1;
	print_ant(ant, vis);
	//game->moves += 1;///not important
	return ;
}

int			ant_cell(void *room, int last)
{
	t_data	*search;
	t_room	*current;
	int		find;
	int		old_find;

	current = (t_room *)room;
	find = 2147483647;
	if (current->flag == last)
		return (-1);
	if (current->full)
		return (-2);
	current->full = 1;
	search = current->paths;
	while (search)
	{
		//printf(".link1 %s\n", ((t_links *)search->data)[0].link1);
		if ((old_find = ant_cell(search->data, last)) < find && old_find != -2)
		{
			//printf("old_find %d %s\n", old_find, ((t_links *)search->data)[0].link1);
			find = 1 + old_find;			
		}
		search = search->next;
	}
	current->full = 0;
//	printf("find %d\n", find);
	return (find == 2147483647 ? -1 : find);
}

		//tmp = ft_joinstr(game->map, output);
		//game->map = ft_strjoin(tmp, "\n");
void		ants_role(t_game *game)
{
	int		result;
	int		calc;
	t_data	*ls;
	t_room	*tmp;
	t_room	*cell;

	calc = 2147483647;
	ls = game->ant_data->room->paths;
	while (ls)
	{
		//printf("go\n");
		tmp = (t_room *)ls->data;
		if (tmp != game->ant_data->last && (tmp->flag == 2 || !tmp->ant))
		{
			//printf("go\n");
			result = ant_cell(tmp, 2);
			if (result < calc && result > -2)
			{
				//printf("newpath %d\n", result);
				calc = result;
				cell = tmp;
				//game->best_data = ft_joinstr(game->best_data, tmp->name);
				//printf("tmp->name %s\n", game->best_data);
			}
		}
		//printf("@\n");
		ls = ls->next;
	}
	if (calc < 2147483647)
		move_ants(game->ant_data, cell, game->visual);
}

	// t_room	*room;
	// t_links	*links;
	// t_data	*tmp_room;
	// t_data	*tmp_path;

	// tmp_room = game->room_data;
	// while (tmp_room != 0)
	// {
	// 	tmp_path = game->path_data;
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

void	free_rooms(void *data)
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

void	free_paths(void *data)
{
	t_links	*path;

	path = (t_links *)data;
	ft_strdel(&path->link1);
	ft_strdel(&path->link2);
	free(data);
	data = NULL;
	return ;
}


int		check_turn(t_ant *ants)
{
	t_room	*path;
	t_data	*list;

	if (ants->room->flag == 2)
		return (0);
	list = ants->room->paths;
	//printf("ants->room->ant %d\n", ants->room->ant);
	while (list && !ants->ant_turn)
	{
		path = (t_room *)list->data;
		//printf("$$$path->ant %d path->ant %d\n", path->ant, path->ant);
		if (path->flag == 2 || (!path->ant && path !=
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

int		ants_ready(t_ant *ants, int numbrs)
{
	int		i;

	i = -1;
	while (++i < numbrs)
		ants[i].ant_turn = 0;
	return (1);
}

t_ant		*l_ant_s(t_data *room, int ants, int str, int fin)
{
	int		i;
	t_ant	*insects;

	i = 0;
	if (!(insects = (t_ant *)malloc(sizeof(t_ant) * ants)))
		lem_error(5);
	while (str < fin)
	{
		insects[i].id = str + 1;
		insects[i].ant_turn = 0;
		insects[i].last = NULL;
		insects[i].room = place_to_chamber(room, 1);
		i++;
		str++;
	}
	return (insects);
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
				game->ant_data += i;;
				ants_role(game);
				game->ant_data -= i;
			}
		}
		ft_putchar('\n');
	}
	if (game->ant_data)
		ft_memdel((void **)&game->ant_data);
}

void		lem_continue_play(t_game *game, int temp_nbr, int tmp_total, int nbr)
{
	int		i;
	int		turn;

	i = 0;
	turn = 1;
	while (tmp_total < game->total)
	{
		if (nbr >= SWARM)
		{
			temp_nbr = SWARM;
			nbr -= SWARM;
			game->ant_data = l_ant_s(game->room_data, SWARM, tmp_total, tmp_total + SWARM);
			tmp_total += SWARM;
		}
		else
		{
			game->ant_data = l_ant_s(game->room_data, nbr, tmp_total, game->total);
			temp_nbr = nbr;
			tmp_total = game->total;
			printf("tmp_total %d\n", tmp_total);
		}
		ants_ready(game->ant_data, temp_nbr);
		printf("temp_nbr %d nbr %d tmp_total %d\n", temp_nbr, nbr, tmp_total);

		lem_role(game, i, turn, temp_nbr);
		turn = 1;
	}
}

void		lem_player(t_game *game)
{	
	int		temp_nbr;	
	int 	nbr;
	int 	tmp_total;	
	
	temp_nbr = 0;
	tmp_total = 0;
	nbr = game->total;
	lem_continue_play(game, temp_nbr, tmp_total, nbr);
	if (game->visual)
	{
		ft_putstr("\033[32m");
		ft_putstr("All ants swarm leaving their nest successfully!\033[0m\n");
	}
}

/*void		lem_player(t_game *game)
{
	int		i;
	int		turn;

	i = -1;
	turn = 1;
	game->ant_data = l_ant_s(game->room_data, game->total);

	ants_ready(game->ant_data, game->total);

	while (turn && !lem_last_ant(game->ant_data, game->total))
	{
		i = -1;
		turn = 0;
		while (++i < game->total)
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
	if (last_insects(game->ant_data, game->total))//SHOULD be ERROR when ret ant.[id] == 1
	{
		printf("error lem_player\n");
		printf("player game->total|%d| lost|%d|\n", game->total, last_insects(game->ant_data, game->total));
		lem_error(3);		 
	}
	printf("player game->total|%d| lost|%d|\n", game->total, last_insects(game->ant_data, game->total));
	if (last_insects(game->ant_data, game->total))
		lem_error(3);
}
2147483647
*/

/*void		lem_player(t_game *game)
{
	int		i;
	int		temp_nbr;
	int		turn;
	int nbr = game->total;
	int tmp_total = 0;

	i = 0;
	turn = 1;
	temp_nbr = 0;
	//int tmp_total_sec = 0;
	while (tmp_total < game->total)
	{
		if (nbr > 10)
		{
			//temp_nbr = nbr - 10;
			temp_nbr = 10;
			nbr -= 10;
			game->ant_data = l_ant_s(game->room_data, 10, tmp_total, tmp_total + 10);
			tmp_total += 10;
		}
		else
		{
			game->ant_data = l_ant_s(game->room_data, nbr, tmp_total, game->total);
			//nbr = game->total - nbr;
			temp_nbr = nbr;
			tmp_total = game->total;
			printf("tmp_total %d\n", tmp_total);
			//tmp_total += nbr;
			//nbr -= temp_nbr;
		}
		ants_ready(game->ant_data, temp_nbr);
		printf("temp_nbrtemp_nbrtemp_nbrtemp_nbrtemp_nbr %d nbr %d tmp_total %d\n", temp_nbr, nbr, tmp_total);
		while (turn && !lem_last_ant(game->ant_data, temp_nbr))
		{
			i = -1;
			turn = 0;			
			while (++i < temp_nbr)
			{
				//printf("nni %d\n", i);
				if (check_turn(game->ant_data + i))
				{
					//printf("nni +");
					turn = 1;
					game->ant_data += i;
					//printf("@@@yei %d\n", i);
					ants_role(game);
					game->ant_data -= i;
				}
			}
			ft_putchar('\n');
		}
		//i += tmp_total;
		//j++;
		turn = 1;
		if (game->ant_data)
			ft_memdel((void **)&game->ant_data);
	}		

		if (last_insects(game->ant_data, game->total))//SHOULD be ERROR when ret ant.[id] == 1
		{
			printf("error lem_player\n");
			printf("player game->total|%d| lost|%d|\n", game->total, last_insects(game->ant_data, game->total));
			lem_error(3);		 
		}
		printf("player game->total|%d| lost|%d|\n", game->total, last_insects(game->ant_data, game->total));
}*/


void		lem_free(t_game *game)
{
	//t_room *room;
	//t_data *tmp_room;

	if (game)
	{	
		if (game->ant_data)
			ft_memdel((void **)&game->ant_data);
		if (game->room_data)
		{
			lem_lst_foreach(game->room_data, free_rooms);
			lem_lst_data_free(game->room_data);
		}
		if (game->path_data)
		{
			lem_lst_foreach(game->path_data, free_paths);
			lem_lst_data_free(game->path_data);
		}
		if (game->map)
			ft_strdel(&game->map);
		ft_memdel((void **)&game);
	}
}

// void		print_room(void *data)
// {
// 	t_room	*room;

// 	if (data != NULL)
// 	{
// 		room = (t_room *)data;
// 		if (room->flag == 1)
// 			ft_putstr("##start\n");
// 		else if (room->flag == 3)
// 			ft_putstr("##end\n");
// 		ft_putstr(room->name);
// 		ft_putchar(' ');
// 		ft_putnbr(room->x);
// 		ft_putchar(' ');
// 		ft_putnbr(room->y);
// 	}
// 	//ft_putchar('\n');
// 	return ;
// }

/*void		print_path(void *data)
{
	t_links	*path;

	path = (t_links *)data;
	ft_putstr(path->link1);
	ft_putchar('-');
	ft_putstr(path->link2);
	ft_putchar('\n');
	return ;
}*/

/*void		lem_print_path(t_game *game)
{
	t_room	*room;
	t_links	*links;
	t_data	*tmp_room;
	t_data	*tmp_path;

	tmp_room = game->room_data;
	while (tmp_room)
	{
		tmp_path = game->path_data;
		room = (t_room *)tmp_room->data;
		while (tmp_path)
		{
			links = (t_links *)tmp_path->data;
			if (!ft_strcmp(links->link1, room->name) &&
				lem_review(game, links->link1))
			{
				printf("links->link2 %s\n", links->link2);
			}
			if (!ft_strcmp(links->link2, room->name) &&
				lem_review(game, links->link2))
			{
				printf("links->link1 %s\n", links->link1);
			}
			tmp_path = tmp_path->next;
		}
		tmp_room = tmp_room->next;
	}
}*/

void		lem_print_path1(t_game *game)
{
	t_room	*tmp;
	t_data	*path;
	t_data *ls;
	t_room	*nm;

	ls = game->ant_data->room->paths;
	while (ls)
	{
		tmp = (t_room *)ls->data;
		path = (t_data *)tmp->paths;
		while (path)
		{
			nm = (t_room *)path->data;
			ft_putstr(nm->name);
			ft_putchar('\n');
			path = path->next;
		}
		//printf("@\n");
		ls = ls->next;
	}
}

// void		lem_print_path2(t_game *game)
// {
// 	//ft_putstr(game->best_data);
// 	printf("game->best_data|%s|\n", game->best_data);
// }


void		lem_print_map(t_game *game)
{
	if (game->visual)
		ft_putstr("\033[31m");
	ft_putnbr(game->total);
	ft_putchar('\n');
	if (game->visual)
		ft_putstr("\033[36m");
	ft_putstr(game->map);
	if (game->visual)
		ft_putstr("\033[0m");
	ft_putchar('\n');

	// ft_putnbr(game->total);
	// ft_putchar('\n');
	// lem_lst_foreach(game->room_data, print_room);
	// lem_lst_foreach(game->path_data, print_path);
	// ft_putchar('\n');
	return ;
}

//int			valid_room_path(t_data *, links->link1)

void		lem_not_cmplinks(t_game *game)
{
	//t_room	*room;
	t_links	*links;
	t_data	*tmp_room;
	t_data	*tmp_path;

	tmp_room = game->room_data;
	while (tmp_room)
	{
		tmp_path = game->path_data;
		//room = (t_room *)tmp_room->data;
		while (tmp_path)
		{
			links = (t_links *)tmp_path->data;
			if (!ft_strcmp(links->link1, links->link2))
			{
				printf("error lem_not_cmplinks\n");
				lem_error(3);
			}
			tmp_path = tmp_path->next;
		}
		tmp_room = tmp_room->next;
	}
}

void		arg_parser(t_game *game, int argc, char **argv)
{
	int		i;

	i = 1;
	while (--argc)
	{
		if (ft_strequ(argv[i], "-v"))
			game->visual = 1;
		else if (ft_strequ(argv[i], "-h"))
			game->map_hide = 1;
		else
			param_error(argv[i], argv[0]);
		i++;
	}
}


int				main(int argc, char **argv)
{
	t_game	*game;

	if(!(game = (t_game *)malloc(sizeof(t_game))))
		lem_error(5);
	lem_struct(game);
	arg_parser(game, argc, argv);
	if (!lem_read_map(game))
	{
		lem_free(game);
		lem_error(3);
	}
	lem_set_path(game);
	lem_not_cmplinks(game);
	if (game->visual == 1)
		display_header();
	if (game->map_hide == 0)
		lem_print_map(game);
	if (game->visual == 1)
		display_header_ant(game->map_hide);
	lem_player(game);
	lem_free(game);
	return (0);
}
