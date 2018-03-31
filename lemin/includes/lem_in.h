/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 15:03:51 by amakhiny          #+#    #+#             */
/*   Updated: 2018/03/20 15:03:58 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# include "../libft/libft.h"
# include "get_next_line.h"
# define SWARM 100000

typedef	struct			s_data
{
	void				*data;
	struct s_data		*next;
}						t_data;

typedef struct			s_links
{
	char				*link1;
	char				*link2;
}						t_links;

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
	int					visual;
	int					map_hide;
	t_ant				*ant_data;
	t_data				*room_data;
	t_data				*path_data;
	char				*map;
}						t_game;

void					lem_parse(int argc, char **argv);
void					lem_player(t_game *game);
void					ants_role(t_game *game);
void					lem_set_path(t_game *game);
void					lem_split_free(char **str);
void					lem_struct(t_game *game);
void					arg_parser(t_game *game, int argc, char **argv);
void					lem_not_cmplinks(t_game *game);
int						lem_read_map(t_game *game);
int						valid_check(t_data *map, char *output);
int						lem_valid_room(char *out);
int						lem_valid_path(t_data *rooms, char *output);
int						ants_ready(t_ant *ants, int numbrs);
t_data					*lem_push(t_data *map, void *param);
t_ant					*l_ant_s(t_data *room, int ants, \
						int str, int fin);
t_room					*room_name(t_data *rooms, char *nbr);
t_room					*build_rooms(char *output, int status);
t_links					*build_paths(char *out);
void					lem_free(t_game *game);
int						valid_rooms(t_data *lst);
void					lem_lst_foreach(t_data *lst, void (*f)());
void					lem_lst_data_free(t_data *first);
void					free_rooms(void *data);
void					free_paths(void *data);
void					display_header(void);
void					display_header_ant(int hide);
void					lem_print_map(t_game *game);
int						lem_symbols(int c);
int						lem_get_ants(void);
int						lem_get_map(t_game *game, int fl, int done, int val);
void					param_error(char *arg, char *argv);
void					lem_error(void);

#endif
