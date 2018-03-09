/*
Great work with the bonus, really like the animation of the ant in the debug. One improvement would be to store the paths instead of re-calc them every time.
*/
#include <stdio.h>
# include "libft/libft.h"

# include <stdarg.h>

# define FT_INT_MIN			(-2147483648)
# define FT_INT_MAX			(2147483647)
/*
** Room macros.
*/

# define IGNORE			(0)
# define STARTROOM 		(1)
# define NORMAL 		(2)
# define ENDROOM		(3)

/*
** File descriptor macros.
*/

# define ERROR			(-1)
# define PARAM_ERR		(-2)
# define MALLC_ERR		(-3)

/*
** ...
*/

# define FT_STD_INT		(0)
# define FT_STD_OUT		(1)
# define FT_STD_ERR		(2)

/*
** Color macros.
*/

# define RESET_COLOUR	(0)
# define LIGHT_BLUE		(36)
# define LIGHT_RED		(91)
# define LIGHT_GREEN	(92)
# define LIGHT_YELLOW	(93)

/*
** Logo and debug ant macros.
*/

# define ANT_NUM		(3)
# define ANT_WIDTH		(47)
# define ANT_HEIGHT		(4)
# define LOGO_WIDTH		(51)
# define LOGO_HEIGHT	(7)

/*
** Debug time macro.
*/

# define DEBUG_SLEEP	(420000)

/*
** Set flag and memdel macro.
*/

# define SET_FLAG(line)	read_command(line);ft_memdel((void **)&line)

/*
** Global variables.
*/

# ifndef GLOBAL_VAR
#  define GLOBAL_VAR

extern char const		g_logo[LOGO_HEIGHT][LOGO_WIDTH];
extern char const		g_ant[ANT_NUM][ANT_HEIGHT][ANT_WIDTH];

# endif

typedef enum		e_bool
{
	FALSE = 0,
	TRUE = 1
}					t_bool;


/*
** Path structure.
*/

typedef struct			s_path
{
	char				*door1;
	char				*door2;
}						t_path;

/*
** Room structure.
*/

typedef struct			s_room
{
	char				*name;
	int					x;
	int					y;
	int					flag;
	int					busy;
	int					has_ant;
	t_list				*paths;
}						t_room;

/*
** Ant structure.
*/

typedef struct			s_ant
{
	int					id;
	t_room				*room;
	t_room				*last;
	int					did_turn;
}						t_ant;

/*
** Parameter structure.
*/

typedef struct			s_param
{
	int					debug;
	int					ant_colored;
	int					path_colored;
	int					room_colored;
}						t_param;

/*
** Lemin structure.
*/

typedef struct			s_lemin
{
	int					ant_total;
	int					rooms_done;
	long int			moves;
	t_ant				*ant_list;
	t_list				*room_list;
	t_list				*path_list;
	t_param				param;
}						t_lemin;




/*
** Lemin functions prototypes.
*/

int						lemin_read(t_lemin *lemin);
int						lemin_validate(t_lemin *lemin);
void					lemin_end(t_lemin *lemin);
void					lemin_loop(t_lemin *lemin);
void					lemin_play(t_lemin *lemin);
void					lemin_start(t_lemin *lemin);
void					lemin_print(t_lemin *lemin);
void					lemin_debug(t_lemin *lemin, t_ant *ant, t_room *room);
t_lemin					*lemin_init(int total_params, char *params[]);
int						find_room(void *room, int flag);
int						get_ant_total(void);
void					get_params(int argc, char *argv[], t_lemin *init);
t_room					*get_room_name(char *name, t_list *list);
t_room					*get_room_flag(int flag, t_list *list);
t_ant					*init_ants(int total_ants, t_list *rooms);
t_path					*init_path(char *line);
t_room					*init_room(char *line, int room_flag);



size_t		ft_strlchr(const char *str, int c)
{
	int		i;

	i = 0;
	while (str[i] && str[i] != (char)c)
		i += 1;
	return (str[i] != (char)c ? -1 : i);
}

int		ft_putchar_fd1(char c, int fd)
{
	return (write(fd, &c, 1));
}

int		ft_putstr_fd1(char const *s, int fd)
{
	int		i;
	int		ret;

	i = 0;
	ret = 0;
	if (s != NULL && ret >= 0)
	{
		while (s[i] && ret >= 0)
			ret = ft_putchar_fd1(s[i++], fd);
	}
	return (ret);
}

int		ft_putnbr_fd1(int n, int fd)
{
	char	c;

	if (n == -2147483648)
		return (ft_putstr_fd1("-2147483648", fd));
	if (n < 0)
	{
		n *= -1;
		ft_putchar_fd1('-', fd);
	}
	if (n < 10)
	{
		c = n + '0';
		ft_putchar_fd1(c, fd);
	}
	else
	{
		ft_putnbr_fd1((n / 10), fd);
		ft_putnbr_fd1((n % 10), fd);
	}
	return (0);
}

int		ft_putendl_fd1(char const *s, int fd)
{
	return ((ft_putstr_fd1(s, fd) + ft_putchar_fd1('\n', fd)));
}

void	ft_puterror_fd(char const *msg, int ret, int fd)
{
	ft_putendl_fd1(msg, fd);
	exit(ret);
}

void		set_color(int c)
{
	ft_putstr_fd1("\x1B[", FT_STD_OUT);
	ft_putnbr_fd1(c, FT_STD_OUT);
	ft_putchar_fd1('m', FT_STD_OUT);
	return ;
}

/*
** ...
*/

void		putcstr(char const *s, size_t col)
{
	set_color(col);
	ft_putstr_fd1(s, FT_STD_OUT);
	set_color(0);
	return ;
}

/*
** ...
*/

void		ft_mini_printf(const char *format, ...)
{
	va_list p;
	char	*tmp;

	va_start(p, format);
	while (*format != '\0')
	{
		if (*format == '%')
		{
			if ((*(format + 1)) == 's')
				putcstr(va_arg(p, char*), va_arg(p, size_t));
			else if ((*(format + 1)) == 'd')
			{
				putcstr(tmp = ft_itoa(va_arg(p, int)), va_arg(p, size_t));
				//putcstr(tmp = ft_itoa(va_arg(p, int)));
				ft_memdel((void**)&tmp);
			}
			format += 1;
		}
		/*else
		{
			if (*format != '\0')
				write(1, format, 1);
		}*/
		format += 1;
	}
	va_end(p);
	return ;
}

void	ft_lstforeach(t_list *lst, void (*f)())
{
	t_list		*tmp;

	tmp = lst;
	while (tmp)
	{
		f(tmp->content);
		tmp = tmp->next;
	}
}

void	ft_lst_rec_free(t_list *first)
{
	if (first)
	{
		ft_lst_rec_free(first->next);
		free(first);
	}
}


t_list		*ft_lstpush(t_list *first, void *item)
{
	t_list	*tmp;

	if (!item)
		return (NULL);
	if (!first)
	{
		first = (t_list *)malloc(sizeof(t_list));
		first->content = item;
		first->next = NULL;
	}
	else
	{
		tmp = first;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = (t_list *)malloc(sizeof(t_list));
		tmp->next->content = item;
		tmp->next->next = NULL;
	}
	return (first);
}





















/*
** 2D String array to store the ascii art of the ants.
*/

char const	g_ant[ANT_NUM][ANT_HEIGHT][ANT_WIDTH] =
{
	{
		{"    \\_/   \n"},
		{"   '-0-'   \n"},
		{"   --0--   \n"},
		{"   .-0-.   \n"},
	},
	{
		{"      \\_/ \n"},
		{"    '-0-'  \n"},
		{"   --0--   \n"},
		{"   .-0-.   \n"},
	},
	{
		{"  \\_/     \n"},
		{"  '-0-'    \n"},
		{"   --0--   \n"},
		{"   .-0-.   \n"},
	}
};

/*
** Display the debug information to the standard out.
*/

void		lemin_debug(t_lemin *lemin, t_ant *ant, t_room *room)
{
	int		i;

	i = 0;
	usleep(DEBUG_SLEEP);
	ft_mini_printf("\n\n");
	while (i < ANT_HEIGHT)
	{
		ft_mini_printf("%s", g_ant[lemin->param.debug % 3][i], LIGHT_YELLOW);
		i += 1;
	}
	ft_mini_printf("\n");
	ft_mini_printf("Ant  id   : %d\n", ant->id, LIGHT_YELLOW);
	ft_mini_printf("Room name : %d\n", room->flag, LIGHT_RED);
	ft_mini_printf("Room flag : %d\n", room->flag, LIGHT_RED);
	ft_mini_printf("L%d-%s \n", ant->id, LIGHT_YELLOW, \
					ant->room->name, LIGHT_RED);
	lemin->param.debug += 1;
	return ;
}



/*
** Lemin logo.
*/

char const	g_logo[LOGO_HEIGHT][LOGO_WIDTH] =
{
	{"    _     U _____ u  __  __              _   _    \n"},
	{"   |.|    \\| ___.|/U|. \\/ .|u   ___     | \\ |.|   \n"},
	{" U | | u   |  __|  \\| |\\/| |/  |_._|   <|  \\| |>  \n"},
	{"  \\| |/__  | |___   | |  | |    | |    U| |\\  |u  \n"},
	{"   |_____| |_____|  |_|  |_|  U/| |\\u   |_| \\_|   \n"},
	{"   //  \\\\  <<   >> <<,-,,-..-,_|___|_,-.||   \\\\,-.\n"},
	{"  (_^)(^_)(__) (__) (./  \\.)\\_)-' '-(_/ (_^)  (_/ \n"}
};

/*
** Prints the help message.
*/

static void	print_help(char *progname)
{
	ft_mini_printf("\n%s\n", g_logo[0], LIGHT_GREEN);
	ft_mini_printf("Lemin reads from the standard input. \n");
	ft_mini_printf("You may write line by or feed in a file, like this:\n");
	ft_mini_printf("%s < test.map\n", progname, LIGHT_BLUE);
	ft_mini_printf("Your may add the following arguments for: \n");
	ft_mini_printf("    -m | --man    -> view the man page\n");
	ft_mini_printf("    -a | --ant    -> color the ants\n");
	ft_mini_printf("    -p | --path   -> color the paths\n");
	ft_mini_printf("    -r | --room   -> color the rooms\n");
	ft_mini_printf("    -d | --debug  -> for debugging purposes\n");
	exit(0);
	return ;
}

/*
** Prints the invalid argument, and exits the program.
*/

static void	invalid_param(char *arg, char *progname)
{
	ft_mini_printf("Argument %s is invalid!\n", arg);
	ft_mini_printf("Run %s -h or --help to know how " \
					"to run this shit. :)\n", progname);
	exit(PARAM_ERR);
	return ;
}

static void	display_manpage(void)
{
	int		ret;

	ret = system("man ./resources/lemin.man");
	exit(ret);
	return ;
}

/*
** Get the parameters from the arguments list.
** Do an action depending on the parameter given.
*/

void		get_params(int argc, char *argv[], t_lemin *init)
{
	int		i;

	i = 1;
	while (--argc)
	{
		if (ft_strequ(argv[i], "-h") || ft_strequ(argv[i], "--help"))
			print_help(argv[0]);
		else if (ft_strequ(argv[i], "-m") || ft_strequ(argv[i], "--man"))
			display_manpage();
		else if (ft_strequ(argv[i], "-d") || ft_strequ(argv[i], "--debug"))
			init->param.debug = TRUE;
		else if (ft_strequ(argv[i], "-a") || ft_strequ(argv[i], "--ant"))
			init->param.ant_colored = TRUE;
		else if (ft_strequ(argv[i], "-p") || ft_strequ(argv[i], "--path"))
			init->param.path_colored = TRUE;
		else if (ft_strequ(argv[i], "-r") || ft_strequ(argv[i], "--room"))
			init->param.room_colored = TRUE;
		else
			invalid_param(argv[i], argv[0]);
		i += 1;
	}
	return ;
}




/*
** Get the total number of ants.
*/

int			get_ant_total(void)
{
	char	*line;
	int		ant_number;

	line = NULL;
	if (get_next_line(0, &line) > 0)
	{
		while (line[0] == '#')
		{
			ft_memdel((void **)&line);
			if (get_next_line(0, &line) == ERROR)
			{
				line = NULL;
				break ;
			}
		}
	}
	ant_number = (line != NULL && *line != '\0') ? ft_atoi(line) : 0;
	ft_memdel((void **)&line);
	return (ant_number);
}




/*
** Find a room, returning the smallest path.
** If none is found, returns an error.
*/

int			find_room(void *room, int targetflag)
{
	t_room	*current;
	t_list	*testing;
	int		smallestpath;
	int		lastpath;

	current = (t_room *)room;
	if (current->flag == targetflag)
		return (0);
	if (current->busy)
		return (-1);
	current->busy = 1;
	smallestpath = FT_INT_MAX;
	testing = current->paths;
	while (testing)
	{
		if ((lastpath = find_room(testing->content, targetflag)) < smallestpath
														&& lastpath != -1)
			smallestpath = 1 + lastpath;
		testing = testing->next;
	}
	current->busy = 0;
	return (smallestpath == FT_INT_MAX ? ERROR : smallestpath);
}





/*
** Get the room by its flag.
*/

t_room		*get_room_flag(int flag, t_list *list)
{
	int		done;
	t_room	*tmp;

	done = 0;
	while (list && !done)
	{
		tmp = (t_room *)list->content;
		done = tmp->flag == flag ? 1 : 0;
		list = list->next;
	}
	return (done ? tmp : NULL);
}


t_room		*get_room_name(char *name, t_list *list)
{
	int		done;
	t_room	*tmp;

	done = 0;
	while (list && !done)
	{
		tmp = (t_room *)list->content;
		done = ft_strequ(name, tmp->name);
		list = list->next;
	}
	return (done ? tmp : NULL);
}


/*
** Initialize the ants list.
*/

t_ant		*init_ants(int total_ants, t_list *rooms)
{
	int		i;
	t_ant	*ants;

	i = 0;
	if (!(ants = (t_ant *)malloc(sizeof(t_ant) * total_ants)))
		ft_puterror_fd("Memory allocation failed.", MALLC_ERR, FT_STD_ERR);
	while (i < total_ants)
	{
		ants[i].room = get_room_flag(STARTROOM, rooms);
		ants[i].did_turn = 0;
		ants[i].id = i + 1;
		i += 1;
	}
	return (ants);
}



/*
** Initialize the path list.
*/

t_path		*init_path(char *line)
{
	int		i;
	t_path	*newpath;

	if (!(newpath = (t_path *)malloc(sizeof(t_path))))
		ft_puterror_fd("Memory allocation failed.", MALLC_ERR, FT_STD_ERR);
	i = (int)ft_strlchr(line, '-');
	newpath->door1 = ft_strsub(line, 0, i);
	newpath->door2 = ft_strdup(line + i + 1);
	free(line);
	return (newpath);
}


/*
** Initialize the room list.
*/

t_room		*init_room(char *line, int room_flag)
{
	int		i;
	t_room	*newroom;

	i = 0;
	if (!(newroom = (t_room *)malloc(sizeof(t_room))))
		ft_puterror_fd("Memory allocation failed.", MALLC_ERR, FT_STD_ERR);
	while (line[i] != ' ')
		i += 1;
	newroom->flag = room_flag;
	newroom->name = ft_strsub(line, 0, i);
	while (line[i] == ' ')
		i += 1;
	newroom->x = ft_atoi(line + i);
	while (line[i] != ' ')
		i += 1;
	while (line[i] == ' ')
		i += 1;
	newroom->y = ft_atoi(line + i);
	newroom->paths = NULL;
	newroom->busy = 0;
	newroom->has_ant = 0;
	free(line);
	return (newroom);
}


/*
** Read and checks if the line is a command.
*/

static int	read_command(char *line)
{
	int		ret;

	ret = NORMAL;
	if (line[0] != '#')
		ret = IGNORE;
	else if (ft_strequ("##start", line))
		ret = STARTROOM;
	else if (ft_strequ("##end", line))
		ret = ENDROOM;
	return (ret);
}

/*
** Read and checks if the line is a room.
*/

static int	read_room(char *line)
{
	//printf("read_room line|%s|\n", line);
	while (*line && *line != ' ')
		line += 1;
	//printf("read_room line|%s|\n", line);
	if (*line != ' ')
		return (0);
	line += 1;
	if (*line == '-')
		line += 1;
	while (*line && ft_isdigit(*line))
		line += 1;
	if (*line != ' ')
		return (0);
	line += 1;
	if (*line == '-')
		line += 1;
	while (*line && ft_isdigit(*line))
		line += 1;
	return (*line != '\0' ? 0 : 1);
}

/*
** Read and checks if the line is a path.
*/

static int	read_path(char *line, t_list *rooms)
{
	int		ret;
	t_path	tmp;

	ret = 0;
	if (ft_strchr(line, '-'))
	{
		tmp.door1 = ft_strsub(line, 0, ft_strlchr(line, '-'));
		tmp.door2 = ft_strdup(line + ft_strlchr(line, '-') + 1);
		if (get_room_name(tmp.door1, rooms) && get_room_name(tmp.door2, rooms))
			ret = 1;
		free(tmp.door1);
		free(tmp.door2);
		tmp.door1 = NULL;
		tmp.door2 = NULL;
	}
	return (ret);
}

/*
** Read a line and check if it's a command, path, or a comment.
** When it reads a line that isn't a command, path, or comment,
** breaks from the while loop.
*/

void	check(t_list *list)
{
	//printf("list->content list->x|%d| list->y|%d|\n", list->x, list->y);
	while (list)
	{
		printf("list->content x|%d| y|%d|\n", ((t_room *)list->content)[0].x, ((t_room *)list->content)[0].y);
		list = list->next;
	}
}

int			lemin_read(t_lemin *lemin)
{
	int		ret;
	char	*line;
	int		flag;

	flag = NORMAL;
	lemin->rooms_done = 0;
	while ((ret = get_next_line(0, &line)) > 0)
	{
		printf("line|%s}\n", line);
		if (read_command(line))
		{
			flag = (flag != NORMAL) ? flag : SET_FLAG(line);
		}
		else if (read_room(line) && !lemin->rooms_done)
		{
			printf("lemin_read room|%s|\n", line);
			lemin->room_list = ft_lstpush(lemin->room_list, \
											init_room(line, flag));
			check(lemin->room_list);
			printf("flag %d\n", flag);
			//printf("@@@ lemin->room_list->content|%d|\n", (int)(t_room *)lemin->room_list->content->x);
			//printf("list->content|%d|\n", lemin->room_list->content->x);
			flag = NORMAL;
		}
		else if (read_path(line, lemin->room_list) && (lemin->rooms_done = 1))
		{
			printf("lemin_read path|%s|\n", line);
			lemin->path_list = ft_lstpush(lemin->path_list, init_path(line));
		}
		else
			break ;
	}
	ft_memdel((void **)&line);
	return (ret);
}



/*
** Check if an ant can move to a room.
*/

static int	check_move(t_ant *ant)
{
	int		yes;
	int		may_move;
	t_list	*ls;
	t_room	*tmp;

	if (ant->room->flag == ENDROOM)
		return (0);
	yes = 1;
	may_move = 0;
	if (ant->did_turn)
		yes = 0;
	ls = ant->room->paths;
	while (yes && ls)
	{
		tmp = (t_room *)ls->content;
		if ((!tmp->has_ant && !(tmp->flag == STARTROOM) && tmp != ant->last)
			|| tmp->flag == ENDROOM)
		{
			may_move = 1;
			break ;
		}
		ls = ls->next;
	}
	return (yes && may_move);
}

/*
** Checks if the lemin loop should end..
*/

static int	end_loop(t_ant *ants, int antnum)
{
	int		i;
	int		ret;

	i = 0;
	ret = 1;
	while (i < antnum && ants != 0)
	{
		if (ants[i].room->flag != ENDROOM)
		{
			ret = 0;
			break ;
		}
		i += 1;
	}
	return (ret);
}

/*
** Reset the ants status (if they have turned).
*/

static void	reset_ants(t_ant *ants, int antnum)
{
	int		i;

	i = 0;
	while (i < antnum)
	{
		ants[i].did_turn = 0;
		i += 1;
	}
	return ;
}












static void	print_ant(t_lemin *lemin, t_ant *ant, t_room *room)
{
	if (lemin->param.debug == FALSE)
	{
		ft_putchar_fd1('L', FT_STD_OUT);
		ft_putnbr_fd1(ant->id, FT_STD_OUT);
		ft_putchar_fd1('-', FT_STD_OUT);
		ft_putstr_fd1(ant->room->name, FT_STD_OUT);
		ft_putchar_fd1(' ', FT_STD_OUT);
	}
	else
		lemin_debug(lemin, lemin->ant_list, room);
	return ;
}

/*
** Move the ants to a room.
*/

static void	move_ants(t_lemin *lemin, t_ant *ant, t_room *room)
{
	ant->room->has_ant = 0;
	ant->last = ant->room;
	ant->room = room;
	ant->room->has_ant = 1;
	print_ant(lemin, ant, room);
	lemin->moves += 1;
	return ;
}

/*
** Starts the game loop.
*/

void		lemin_play(t_lemin *lemin)
{
	int		result;
	int		distance;
	t_list	*ls;
	t_room	*tmp;
	t_room	*next;

	distance = FT_INT_MAX;
	ls = lemin->ant_list->room->paths;
	while (ls)
	{
		tmp = (t_room *)ls->content;
		if ((tmp->flag == ENDROOM || !tmp->has_ant) \
				&& tmp != lemin->ant_list->last)
		{
			result = find_room(tmp, ENDROOM);
			if (result < distance && result > -1)
			{
				distance = result;
				next = tmp;
			}
		}
		ls = ls->next;
	}
	if (distance < FT_INT_MAX)
		move_ants(lemin, lemin->ant_list, next);
}

/*
** Check the ants, sand see if they can move.
*/

static void	check_ants(t_lemin *lemin)
{
	int		i;
	int		turn;

	turn = 0;
	while (!turn && !end_loop(lemin->ant_list, lemin->ant_total))
	{
		i = 0;
		turn = 1;
		while (i < lemin->ant_total)
		{
			if (check_move(lemin->ant_list + i))
			{
				turn = 0;
				lemin->ant_list += i;
				lemin_play(lemin);
				lemin->ant_list -= i;
			}
			i += 1;
		}
		ft_putchar_fd1('\n', FT_STD_OUT);
	}
	return ;
}

/*
** Loop through
*/

void		lemin_loop(t_lemin *lemin)
{
	while (!end_loop(lemin->ant_list, lemin->ant_total))
	{
		reset_ants(lemin->ant_list, lemin->ant_total);
		check_ants(lemin);
	}
	if (lemin->param.debug != FALSE)
		ft_mini_printf("\nNumber of moves : %d\n", lemin->moves, LIGHT_GREEN);
	lemin_end(lemin);
	return ;
}








/*
** Delete all the rooms from the list.
*/

static void	delete_rooms(void *data)
{
	t_room	*room;

	room = (t_room *)data;
	ft_strdel(&room->name);
	ft_lst_rec_free(room->paths);
	free(data);
	data = NULL;
	return ;
}

/*
** Delete all the paths from the list.
*/

static void	delete_paths(void *data)
{
	t_path	*path;

	path = (t_path *)data;
	ft_strdel(&path->door1);
	ft_strdel(&path->door2);
	free(data);
	data = NULL;
	return ;
}

/*
** This is the end.
** Free everything, so we don't have memory leaks. :)
** If the debug option is on, display the total number of moves.
*/

void		lemin_end(t_lemin *lemin)
{
	if (lemin)
	{
		if (lemin->room_list)
		{
			ft_lstforeach(lemin->room_list, delete_rooms);
			ft_lst_rec_free(lemin->room_list);
		}
		if (lemin->path_list)
		{
			ft_lstforeach(lemin->path_list, delete_paths);
			ft_lst_rec_free(lemin->path_list);
		}
		if (lemin->ant_list)
			ft_memdel((void **)&lemin->ant_list);
		ft_memdel((void **)&lemin);
	}
	return ;
}




/*
** Validate the rooms names.
*/

static int	validate_rooms_name(t_list *rooms)
{
	int		ret;
	t_room	*tmp;

	ret = 0;
	if (rooms != 0)
	{
		while (rooms)
		{
			tmp = (t_room *)rooms->content;
			if (tmp->name[0] == '#' || tmp->name[0] == 'L' \
				|| ft_strchr(tmp->name, '-') != 0)
				break ;
			rooms = rooms->next;
		}
		ret = 1;
	}
	return (ret && !rooms);
}

/*
** Validate the rooms flag.
*/

static int	validate_rooms_flag(t_list *rooms, t_list *paths)
{
	int		start;
	int		end;
	t_room	*tmp;

	start = 0;
	end = 0;
	if (rooms != 0 || paths != 0)
	{
		while (rooms)
		{
			tmp = (t_room *)rooms->content;
			if (tmp->flag == STARTROOM)
				start += 1;
			if (tmp->flag == ENDROOM)
				end += 1;
			rooms = rooms->next;
		}
	}
	return (start == 1 && end == 1);
}

/*
** Validate the paths.
*/

static int	validate_paths(t_room *rooms, t_list *paths)
{
	return ((rooms != 0 && paths != 0) && find_room(rooms, ENDROOM) >= 0);
}

/*
** Validate the path and the flags.
*/

int			lemin_validate(t_lemin *lemin)
{
	return (validate_paths(get_room_flag(STARTROOM, lemin->room_list), \
										lemin->path_list) \
			&& validate_rooms_flag(lemin->room_list, lemin->path_list) \
			&& validate_rooms_name(lemin->room_list));
}





/*
** Starts going from every room and path in the list, before being validated.
*/

void		lemin_start(t_lemin *lemin)
{
	t_room	*room;
	t_path	*path;
	t_list	*tmp_room;
	t_list	*tmp_path;

	tmp_room = lemin->room_list;
	while (tmp_room != 0)
	{
		room = (t_room *)tmp_room->content;
		tmp_path = lemin->path_list;
		while (tmp_path != 0)
		{
			path = (t_path *)tmp_path->content;
			if (ft_strequ(path->door1, room->name))
				room->paths = ft_lstpush(room->paths, \
						get_room_name(path->door2, lemin->room_list));
			if (ft_strequ(path->door2, room->name))
				room->paths = ft_lstpush(room->paths, \
						get_room_name(path->door1, lemin->room_list));
			tmp_path = tmp_path->next;
		}
		tmp_room = tmp_room->next;
	}
	return ;
}




static void	print_room(void *room)
{
	t_room	*tmp_room;

	if (room != NULL)
	{
		tmp_room = (t_room *)room;
		if (tmp_room->flag == STARTROOM)
			ft_putendl_fd1("##start", FT_STD_OUT);
		else if (tmp_room->flag == ENDROOM)
			ft_putendl_fd1("##end", FT_STD_OUT);
		ft_putstr_fd1(tmp_room->name, FT_STD_OUT);
		ft_putchar_fd1(' ', FT_STD_OUT);
		ft_putnbr_fd1(tmp_room->x, FT_STD_OUT);
		ft_putchar_fd1(' ', FT_STD_OUT);
		ft_putnbr_fd1(tmp_room->y, FT_STD_OUT);
	}
	ft_putchar_fd1('\n', FT_STD_OUT);
	return ;
}

/*
** Print the paths.
*/

static void	print_path(void *tunnel)
{
	t_path	*path;

	path = (t_path *)tunnel;
	ft_putstr_fd1(path->door1, FT_STD_OUT);
	ft_putchar_fd1('-', FT_STD_OUT);
	ft_putendl_fd1(path->door2, FT_STD_OUT);
	return ;
}

/*
** Print the rooms (colored).
*/

static void	print_room_colored(void *room)
{
	t_room	*tmp;

	if (room != NULL)
	{
		tmp = (t_room *)room;
		if (tmp->flag == STARTROOM)
			ft_putendl_fd1("##start", FT_STD_OUT);
		else if (tmp->flag == ENDROOM)
			ft_putendl_fd1("##end", FT_STD_OUT);
		ft_mini_printf("%s %d %d", tmp->name, LIGHT_RED, \
						tmp->x, LIGHT_GREEN, tmp->y, LIGHT_GREEN);
	}
	ft_putchar_fd1('\n', FT_STD_OUT);
	return ;
}

/*
** Print the paths (colored).
*/

static void	print_path_colored(void *tunnel)
{
	t_path	*path;

	path = (t_path *)tunnel;
	ft_mini_printf("%s-%s\n", path->door1, LIGHT_BLUE, \
					path->door2, LIGHT_BLUE);
	return ;
}

/*
** Print to the stdout the rooms and paths.
*/

void		lemin_print(t_lemin *lemin)
{
	if (lemin->param.ant_colored == FALSE)
		ft_putnbr_fd1(lemin->ant_total, FT_STD_OUT);
	else
		ft_mini_printf("%d", lemin->ant_total, LIGHT_YELLOW);
	ft_putchar_fd1('\n', FT_STD_OUT);
	if (lemin->param.room_colored == FALSE)
		ft_lstforeach(lemin->room_list, print_room);
	else
		ft_lstforeach(lemin->room_list, print_room_colored);
	if (lemin->param.path_colored == FALSE)
		ft_lstforeach(lemin->path_list, print_path);
	else
		ft_lstforeach(lemin->path_list, print_path_colored);
	ft_putchar_fd1('\n', FT_STD_OUT);
	return ;
}



/*
** Initialize the total number of ants, room, path and ant lists.
*/

static void	init_values(t_lemin *init)
{
	if ((init->ant_total = get_ant_total()) == FALSE)
		ft_puterror_fd("Error: Invalid number of ants.", ERROR, FT_STD_ERR);
	if (lemin_read(init) == ERROR)
		ft_puterror_fd("Error: Reading failed.", ERROR, FT_STD_ERR);
	if (!init->room_list || !init->path_list)
		ft_puterror_fd("Error: Missing rooms or paths.", ERROR, FT_STD_ERR);
	init->ant_list = init_ants(init->ant_total, init->room_list);
}

/*
** Initialize the lemin structure.
*/

t_lemin		*lemin_init(int total_params, char *params[])
{
	t_lemin	*init;

	if (!(init = (t_lemin *)malloc(sizeof(t_lemin))))
		ft_puterror_fd("Memory allocation failed.", MALLC_ERR, FT_STD_ERR);
	init->moves = 0;
	init->room_list = NULL;
	init->path_list = NULL;
	init->param.debug = FALSE;
	init->param.ant_colored = FALSE;
	init->param.path_colored = FALSE;
	init->param.room_colored = FALSE;
	get_params(total_params, params, init);
	init_values(init);
	return (init);
}


int			main(int argc, char *argv[])
{
	t_lemin	*lemin;

	lemin = lemin_init(argc, argv);
	lemin_print(lemin);
	lemin_start(lemin);
	if (lemin_validate(lemin) != TRUE)
	{
		lemin_end(lemin);
		ft_puterror_fd("Error: Something went wrong, check your input.", \
							ERROR, FT_STD_ERR);
	}
	lemin_loop(lemin);
	return (0);
}



