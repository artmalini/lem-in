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
	int					busy;
	int					has_ant;
	t_data				*paths;
}						t_room;

typedef struct			s_ant
{
	int					id;
	t_room				*room;
	t_room				*last;
	int					did_turn;
}						t_ant;

typedef struct			s_game
{
	int					ant_total;
	int					rooms_done;
	long int			moves;
	t_ant				*ant_list;
	t_data				*room_list;
	t_data				*path_list;
}						t_game;

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
	if (!(res = (t_data*)malloc(sizeof(t_data))))
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
	if (get_next_line(0, &output) < 1)
		lem_error(3);
	//printf("@|%s|\n", output);
	val = ft_atoi(output);
	while (ft_isdigit(output[++i]));
	//i -= 1;
	//while (ft_isspace(output[++i]));
	//printf("@|%c|\n", output[i]);
	if (output[i] != '\0')
		lem_error(3);
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

t_room		*build_room(char *output, int status)
{
	char	**rooms;
	t_room	*param;

	if (!(param = (t_room *)malloc(sizeof(t_room))))
		lem_error(5);
	rooms = ft_strsplit(output, ' ');
	param->name = ft_strdup(rooms[0]);/////////////////LEAK
	param->x = ft_atoi(rooms[1]);
	param->y = ft_atoi(rooms[2]);
	param->flag = 0;
	param->busy = 0;
	param->has_ant = 0;
	param->paths = NULL;
	lem_split_free(rooms);
	printf("rooms|%s| status|%d|\n", param->name, status);

	return (param);
}
/*void	build_room(char *output, int status)
{
	char	**rooms;
	t_room	*param;

	if (!(param = (t_room *)malloc(sizeof(t_room))))
		lem_error(5);
	rooms = ft_strsplit(output, ' ');
	param->name = ft_strdup(rooms[0]);
	param->x = ft_atoi(rooms[1]);
	param->y = ft_atoi(rooms[2]);
	param->flag = 0;
	param->busy = 0;
	param->has_ant = 0;
	param->paths = NULL;
	lem_split_free(rooms);
	printf("rooms|%s| status|%d|\n", param->name, status);
}*/

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
	//printf("valid |%s|\n", out);
	while (ft_isalnum(*out) && *out != 'L' && *out != '#')
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
	if (*out != '\0')
		return (0);
	//printf("valid |%s|\n", out);
	return (1);
}


void	check(t_data *map)
{
	//printf("check map->x|%d| map->y|%d|\n", ((t_room *)map->data)[0].x, ((t_room *)map->data)[0].y);
	while (map)
	{
		printf("check map->x|%d| map->y|%d|\n", ((t_room *)map->data)[0].x, ((t_room *)map->data)[0].y);
		map = map->next;
	}
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
	game->rooms_done = 0;
	while ((val = get_next_line(0, &output)) > 0)
	{		
		if (check_hash(output))
		{
			map_check_hash(&output, &status);
			//printf("@%s\n", output);
			// if (status == 2)
			// {
			// 	status = check_hash(output);
			// 	ft_memdel((void **)&output);
			// }
			// else
			// 	status = status;
		}
		else if (lem_valid_room(output) && !game->rooms_done)
		{
			//build_room(output, status);
			game->room_list = ft_lem_push(game->room_list, build_room(output, status));
			 check(game->room_list);////@@@@@@@@@@@@@
			status = 2;
		}
		else
			break ;
		ft_memdel((void **)&output);
	}
	ft_memdel((void **)&output);
	printf("end map val %d\n", val);
	return (val);
}

static int		lem_read_map(t_game *game)
{
	game->ant_total = lem_get_ants();
	printf("game->ant_total|%d|\n", game->ant_total);
	
	if (game->ant_total == 0 || !lem_get_map(game))
		return (3);	

	return (1);
}

static void		lem_struct(t_game *game)
{
	game->ant_total = 0;
	game->rooms_done = 0;
	game->moves = 0;
	game->ant_list = NULL;
	game->room_list = NULL;
	game->path_list = NULL;
	//return (lem);
}

static void		lem_parse(int argc, char **argv)
{
	t_game	*game;

	while (--argc)
		param_error(argv[1], argv[0]);
	if(!(game = (t_game *)malloc(sizeof(t_game))))
		lem_error(5);
	lem_struct(game);
	if (lem_read_map(game) == INVALID_MAP)
		lem_error(3);

	ft_memdel((void**)&game);
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
	//close(fd);	
	return (0);
}
