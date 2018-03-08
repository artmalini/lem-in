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

static int		ft_isspace(char c)
{
	if (c == 32 || c == '\n' || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}

static void		lemin_error(int error)
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

static t_game	*ft_lem_push(t_data *map, t_game *param)
{}

static int		lemin_get_ants(void)
{
	int		i;
	int		val;
	char	*output;

	i = -1;
	if (get_next_line(0, &output) < 1)
		lemin_error(3);
	//printf("@|%s|\n", output);
	val = ft_atoi(output);
	while (ft_isdigit(output[++i]));
	i -= 1;
	while (ft_isspace(output[++i]));
	//printf("@|%c|\n", output[i]);
	if (output[i] != '\0')
		lemin_error(3);
	//printf("@|%s|\n", output);
	ft_memdel((void**)&output);
	return (val);
}

static int		check_hash(char *output)
{
	if (output[0] != '#')
		return (0);
	else if (ft_strequ("##start", output))
		return (1);
	else if (ft_strequ("##end", output))
		return (3);
	else
		return (2);
}

static t_room	*build_room(char *output, int status)
{
	char	**rooms;
	t_room	*param;

	if (!(param = (t_room *)malloc(sizeof(t_room))))
		lemin_error(5);
	rooms = ft_strsplit(output, ' ');

	return (param);
}

void	check(t_data *map)
{
	while (map)
	{
		printf("check |%s|\n", (t_game *)map->data[0].y);
		map = map->next;
	}
}

static int		lemin_get_map(t_game *game)
{
	int		status;
	char	*output;

	status = 0;
	game->rooms_done = 0;
	while (get_next_line(0, &output) > 0)
	{
		if (check_hash(output))
		{
			status = check_hash(output);
			printf("status|%d|\n", status);
		}
		else if (lemin_room(output) && !game->rooms_done)
		{
			game->room_list = ft_lem_push(game->room_list, build_room(output, status));
			check(game->room_list);
		}
		ft_memdel((void **)&output);
	}
	ft_memdel((void **)&output);
	return (1);
}

static int		lemin_read_map(t_game *game)
{
	game->ant_total = lemin_get_ants();
	printf("game->ant_total|%d|\n", game->ant_total);
	
	if (game->ant_total == 0 || !lemin_get_map(game))
		return (3);	

	return (1);
}

static void		lemin_struct(t_game *game)
{
	game->ant_total = 0;
	game->rooms_done = 0;
	game->moves = 0;
	game->ant_list = NULL;
	game->room_list = NULL;
	game->path_list = NULL;
	//return (lem);
}

static void		lemin_parse(int argc, char **argv)
{
	t_game	*game;

	while (--argc)
		param_error(argv[1], argv[0]);
	if(!(game = (t_game *)malloc(sizeof(t_game))))
		lemin_error(5);
	lemin_struct(game);
	if (lemin_read_map(game) == INVALID_MAP)
		lemin_error(3);

	ft_memdel((void**)&game);
}

int				main(int argc, char **argv)
{
	//int		fd;

	//printf("%d\n", argc);
	//if (argc > 1)
	//	lemin_error(2);
	if (argc < 1)
		lemin_error(1);
	//else if(!(fd = open(argv[0], O_RDONLY)))
	//	lemin_error(3);
	else
		lemin_parse(argc, argv);
	//close(fd);	
	return (0);
}
