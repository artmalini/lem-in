#include "includes/lem_in.h"

void		lem_struct(t_game *game)
{
	game->total = 0;
	game->done = 0;
	game->visual = 0;
	game->map_hide = 0;
	game->ant_data = NULL;
	game->room_data = NULL;
	game->path_data = NULL;
	game->map = NULL;
}

int		lem_read_map(t_game *game)
{
	game->total = lem_get_ants();
	if (game->total == 0 || lem_get_map(game, 3))
		return (0);
	if (!valid_rooms(game->room_data))
		return (0);	
	return (1);
}

void		lem_not_cmplinks(t_game *game)
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
			if (!ft_strcmp(links->link1, links->link2))
				lem_error();
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
			param_error(argv[1], argv[0]);
		i++;
	}
}

void		lem_parse(int argc, char **argv)
{
	t_game	*game;

	if(!(game = (t_game *)malloc(sizeof(t_game))))
		lem_error();
	lem_struct(game);
	arg_parser(game, argc, argv);
	if (!lem_read_map(game))
	{
		lem_free(game);
		lem_error();
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
}
