/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/24 11:06:55 by amakhiny          #+#    #+#             */
/*   Updated: 2018/02/24 11:06:58 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <mlx.h>
# include "../libft/libft.h"
# include "get_next_line.h"

# define H			1000
# define W			1600
# define UP			126
# define DOWN		125
# define RIGHT		124
# define LEFT		123
# define PLUS		69
# define MINUS		78
# define ABS(v)		((v) > 0 ? (v) : -(v))

typedef struct		s_point
{
	int				x;
	int				y;
}					t_place;

typedef struct		s_viz
{
	int				**card;
	int				**hex;
	int				ycard;
	int				xcard;
	int				size;
	int				pos_x;
	int				pos_y;
	double			zoom;
	int				r;
	int				g;
	int				b;
	char			*drw;
	int				e;
	int				bi;
	int				l;
	void			*mlx;
	void			*win;
	void			*img;
}					t_vis;

int					map_check(int fd);
int					map_check2(char *out);
int					hex_int(char *hex);
int					print_err(char *err);
void				free_int_map(t_vis *prm, int size);
void				free_map(char **map, int size);
void				build_card(t_vis *prm, int fd, char *output);
void				fit_color(t_vis *prm, int x, int y);
int					keyboard_vis_hook(int key, t_vis *prm);
int					mouse_vis_hook(int key, int x, int y, t_vis *prm);
int					valid_hex(char *str);
int					image_builder(t_vis *prm);
void				vis_map(t_vis *prm);
void				print_help(t_vis *prm);

#endif
