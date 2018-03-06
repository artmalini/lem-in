
#include <stdio.h>


# include "libft/libft.h"
# include <limits.h>
# define CUR(a) ((a)->content)
# define NXT(a) (a)->next->content
# define ABS(a) ((a < 0) ? a * -1 : a);
# define HASH 5000
# define Q_EMPTY(q) ((q)->start == (q)->end)
# define LEGAL(a) (a != '#' && a != 'L')


typedef struct		s_ant
{
	int				id;
	int				path_len;
	char			*room;
	char			**path;
}					t_ant;

typedef struct		s_room
{
	int				num_ants;
	int				x;
	int				y;
	int				dist;
	int				num_adj;
	char			*r_name;
	char			**adj;
}					t_room;

typedef struct		s_htable
{
	int				size;
	t_list			**keys;
	t_list			**values;
}					t_htable;

typedef struct		s_q
{
	char			**buf;
	size_t			start;
	size_t			end;
	size_t			size;
}					t_q;

t_htable		*h_new(void);
void			h_insert(t_htable **h, t_room *r);
int				h_index(t_htable *h, char *k);
void			h_del(t_htable **h);

void			char_del(void *content, size_t size);
void			room_del(void *content, size_t size);
void			ants_del(t_ant ***ants, int tot);

t_htable		*init_rooms(t_room **start, t_room **end, t_ant ***ants);
void			add_adj(char *cur, t_htable **out);

void			find_path(t_ant ***ants, t_htable **f, t_room **s, t_room **e);
void			next_room(t_ant **ant, t_room **s, t_room **n);
t_room			*get_room(t_htable **h, char *k);
t_ant			*init_ant(int i, t_room **s);

void			bfs(t_htable *h, char *s);
t_q				*new_q(void);
void			push(t_q *q, char *c);
char			*pop(t_q *q);

int				is_valid(char *c, t_htable *f);


static int	ft_isint(char *s)
{
	int l;

	l = ft_strlen(s);
	if (*s == '+' || *s == '-')
		s++;
	if (!*s)
		return (0);
	if (l >= 10 && ft_strcmp(s, "2147483648") >= 0)
		return (0);
	while (*s)
		if (!ft_isdigit(*s++))
			return (0);
	return (1);
}


void			char_del(void *content, size_t size)
{
	if (content)
		free(content);
	size = 0;
}

void			room_del(void *content, size_t size)
{
	int		i;
	t_room	*ptr;

	if (content)
	{
		ptr = (t_room *)content;
		i = -1;
		if (ptr->adj)
			while (++i <= ptr->num_adj)
				free(ptr->adj[i]);
		free(ptr->r_name);
		free(ptr->adj);
		free(ptr);
	}
	size = 0;
}

void			ants_del(t_ant ***ants, int tot)
{
	int i;
	int j;

	if (ants[0])
	{
		i = -1;
		while (++i < tot)
		{
			if (ants[0][i])
			{
				j = -1;
				if ((ants[0][i])->path)
				{
					while (++j < (ants[0][i])->path_len)
						free((ants[0][i])->path[j]);
					free(ants[0][i]->path);
				}
				free(ants[0][i]);
			}
		}
		free(ants[0]);
	}
}





static int		is_unique(char *c, t_htable *f)
{
	t_list *cur;
	int i;
	char **ent;
	int out;

	if (f->size == 0)
		return (1);
	ent = ft_strsplit(c, ' ');
	i = h_index(f, ent[0]);
	out = 1;
	cur = f->keys[i];
	while (cur != NULL && !ft_strequ(CUR(cur), ent[0]))
		cur = cur->next;
	if (cur && ft_strequ(CUR(cur), ent[0]))
		out = 0;
	free(ent[0]);
	free(ent[1]);
	free(ent[2]);
	free(ent);
	return (out);
}

int				is_valid(char *c, t_htable *f)
{
	int i;
	int spaces;
	int hyphens;

	i = -1;
	spaces = 0;
	hyphens = 0;
	if (c[0] == 'L')
		return (0);
	while (c[++i] != '\0')
	{
		spaces += (c[i] == ' ') ? 1 : 0;
		hyphens += (c[i] == '-') ? 1 : 0;
	}
	if (c[0] == '#')
		return (1);
	else if (spaces == 2 && hyphens == 0)
		return (is_unique(c, f));
	else if (hyphens == 1 && spaces == 0)
		return (1);
	return (0);
}

static void		adj_add(t_room **a, t_room **b)
{
	char	**a_new;
	int		i;

	a_new = ft_memalloc(((*a)->num_adj + 2) * sizeof(char *));
	i = -1;
	while (++i < (*a)->num_adj && (*a)->adj && (*a)->adj[i] != NULL)
	{
		a_new[i] = ft_strdup((*a)->adj[i]);
		ft_strdel(&((*a)->adj[i]));
	}
	a_new[i] = ft_strdup((*b)->r_name);
	ft_strdel(&((*a)->adj[i]));
	if ((*a)->adj)
		free((*a)->adj);
	(*a)->adj = a_new;
	//printf("(*a)->adj  %s\n", a_new[i]);
}

void			add_adj(char *cur, t_htable **out)
{
	char	**l;
	int		i;
	t_room	*a;
	t_room	*b;

	l = ft_strsplit(cur, '-');
	i = 0;
	while (l[i] != NULL)
		i++;
	if (i == 2 && l[0] && l[1])
	{
		a = get_room(out, l[0]);
		b = get_room(out, l[1]);
		if (a && b)
		{
			adj_add(&a, &b);
			adj_add(&b, &a);
			a->num_adj++;
			b->num_adj++;
		}
	}
	while (i >= 0)
		ft_strdel(&l[i--]);
	free(l);
}





t_q				*new_q(void)
{
	t_q	*out;

	out = (t_q *)ft_memalloc(sizeof(t_q));
	out->size = 4;
	out->buf = (char **)ft_memalloc(out->size * sizeof(char *));
	out->start = 0;
	out->end = 0;
	return (out);
}

void			push(t_q *q, char *c)
{
	char	**new_buf;
	size_t	i;

	if (q->end >= q->size)
		q->end = 0;
	q->buf[q->end++] = ft_strdup(c);
	if (q->end == q->size)
	{
		new_buf = (char **)ft_memalloc(q->size * 2 * sizeof(char *));
		i = -1;
		while (++i < q->size)
		{
			new_buf[i] = ft_strdup(q->buf[i]);
			ft_strdel(&(q->buf[i]));
		}
		free(q->buf);
		q->buf = new_buf;
		q->size *= 2;
	}
}

char			*pop(t_q *q)
{
	char	*out;
	char	**new_buf;
	size_t	i;

	if (Q_EMPTY(q))
		return (NULL);
	out = ft_strdup(q->buf[q->start++]);
	ft_strdel(&(q->buf[q->start - 1]));
	if (q->start >= q->size)
	{
		q->start = 0;
		if (q->size >= HASH / 2 && q->end < q->size / 2)
		{
			new_buf = (char **)ft_memalloc((q->size / 2) * sizeof(char *));
			i = -1;
			while (++i <= q->end)
			{
				new_buf[i] = ft_strdup(q->buf[i]);
				ft_strdel(&(q->buf[i]));
			}
			free(q->buf);
			q->buf = new_buf;
		}
	}
	return (out);
}




static void		step_next(t_ant **ant, t_htable **f, t_room **e)
{
	t_room	*n;
	t_room	*c;
	int		i;

	i = -1;
	if (ft_strequ((*ant)->room, (*e)->r_name))
		return ;
	c = get_room(f, (*ant)->room);
	while (++i < c->num_adj)
	{
		n = get_room(f, c->adj[i]);
		if (n && ((n->num_ants >= 0 && ft_strequ((*e)->r_name, n->r_name))
				|| (n->num_ants == 0 && n->dist <= c->dist)))
			break ;
	}
	if (i < c->num_adj)
		next_room(ant, &c, &n);
}

void			find_path(t_ant ***ants, t_htable **f, t_room **s, t_room **e)
{
	int		i;
	int		tot_ants;
	int		p_path;
	t_room	*end;

	tot_ants = (*s)->num_ants;
	end = get_room(f, (*e)->r_name);
	i = -1;
	while (++i < tot_ants)
		ants[0][i] = init_ant(i, s);
	bfs(*f, (*e)->r_name);
	while (end->num_ants < tot_ants)
	{
		i = -1;
		while (++i < tot_ants)
		{
			p_path = (ants[0][i])->path_len;
			step_next(&(ants[0][i]), f, e);
			if (p_path < (ants[0][i])->path_len)
				printf("L%d-%s ", (ants[0][i])->id, (ants[0][i])->room);
		}
		ft_putstr("\b\n");
	}
	ants_del(ants, tot_ants);
}

void			next_room(t_ant **ant, t_room **c, t_room **n)
{
	char	**n_path;
	int		i;

	n_path = (char **)ft_memalloc(((*ant)->path_len + 1) * sizeof(char *));
	(*c)->num_ants--;
	(*ant)->room = (*n)->r_name;
	(*n)->num_ants++;
	i = -1;
	while (++i < (*ant)->path_len)
	{
		n_path[i] = ft_strdup((*ant)->path[i]);
		ft_strdel(&(*ant)->path[i]);
	}
	n_path[i] = ft_strdup((*n)->r_name);
	free((*ant)->path);
	(*ant)->path = n_path;
	(*ant)->path_len++;
}

t_room			*get_room(t_htable **h, char *k)
{
	int		i;
	t_list	*curr;
	t_room	*c;

	if (!k || !LEGAL(k[0]))
		return (NULL);
	i = h_index(*h, k);
	curr = (*h)->values[i];
	if (curr)
	{
		c = (t_room *)CUR(curr);
		while (curr && !ft_strequ(c->r_name, k))
		{
			curr = curr->next;
			c = (t_room *)CUR(curr);
		}
		return (CUR(curr));
	}
	return (NULL);
}

t_ant			*init_ant(int i, t_room **s)
{
	t_ant *out;

	out = (t_ant *)ft_memalloc(sizeof(t_ant));
	out->id = i + 1;
	out->room = (*s)->r_name;
	out->path_len = 1;
	out->path = (char **)ft_memalloc(sizeof(char *));
	out->path[0] = ft_strdup((*s)->r_name);
	return (out);
}


static void		del_q(t_q **q)
{
	int i;

	i = -1;
	while (++i < (int)(*q)->size)
		free(((*q)->buf[i]));
	free((*q)->buf);
	free(*q);
}

static void		search(t_q **q, t_htable *h)
{
	t_room	*c;
	t_room	*n;
	int		d;
	int		i;
	char	*p;

	d = -1;
	while (!Q_EMPTY(*q))
	{
		i = -1;
		p = pop(*q);
		c = get_room(&h, p);
		c->dist = ++d;
		while (++i < c->num_adj)
		{
			n = get_room(&h, c->adj[i]);
			if (n && n->dist == INT_MAX)
			{
				n->dist = d + 1;
				push(*q, n->r_name);
			}
		}
		ft_strdel(&p);
	}
}

void			bfs(t_htable *h, char *s)
{
	t_q		*q;

	q = new_q();
	push(q, s);
	search(&q, h);
	del_q(&q);
}



static t_list	*val_lstnew(t_room *f)
{
	t_room *val;
	t_list *out;

	val = (t_room *)ft_memalloc(sizeof(t_room));
	out = (t_list *)ft_memalloc(sizeof(t_list));
	val->num_ants = f->num_ants;
	val->x = f->x;
	val->y = f->y;
	val->r_name = ft_strdup(f->r_name);
	val->num_adj = f->num_adj;
	val->dist = INT_MAX;
	out->content = (void *)val;
	out->content_size = sizeof(t_room);
	out->content_size += (ft_strlen(f->r_name) + 1) * sizeof(char);
	out->next = NULL;
	return (out);
}

t_htable		*h_new(void)
{
	t_htable	*out;
	int			i;

	out = (t_htable *)malloc(sizeof(t_htable));
	out->keys = (t_list **)ft_memalloc(HASH * sizeof(t_list *));
	out->values = (t_list **)ft_memalloc(HASH * sizeof(t_list *));
	i = -1;
	while (++i < HASH)
	{
		out->keys[i] = NULL;
		out->values[i] = NULL;
	}
	out->size = 0;
	return (out);
}

void			h_insert(t_htable **h, t_room *f)
{
	int		i;
	t_list	*key;
	t_list	*val;
	t_list	*curr;

	i = h_index(*h, f->r_name);
	curr = (*h)->keys[i];
	while (curr && !ft_strequ(((char *)CUR(curr)), f->r_name))
		curr = curr->next;
	if (!curr)
	{
		key = ft_lstnew((f->r_name), ft_strlen(f->r_name) * sizeof(char) + 1);
		val = val_lstnew(f);
		if ((*h)->keys[i] == NULL)
			(*h)->keys[i] = key;
		else
			ft_lstadd(&((*h)->keys[i]), key);
		if ((*h)->values[i] == NULL)
			(*h)->values[i] = val;
		else
			ft_lstadd(&((*h)->values[i]), val);
		(*h)->size++;
	}
}

int				h_index(t_htable *h, char *k)
{
	int		i;
	int		j;
	t_list	*curr;

	i = 0;
	j = -1;
	while (k[++j] != '\0')
		i += k[j];
	i = ABS(i % HASH);
	while (h->keys[i])
	{
		curr = h->keys[i];
		while (curr)
		{
			if (ft_strequ((char *)CUR(curr), k))
				return (i);
			curr = curr->next;
		}
		i = ABS((i + 1) % HASH);
	}
	return (i);
}

void			h_del(t_htable **h)
{
	int		i;
	t_list	*k;
	t_list	*v;

	i = -1;
	if (*h)
	{
		while (++i < HASH)
		{
			k = (*h)->keys[i];
			v = (*h)->values[i];
			if (k)
				ft_lstdel(&k, char_del);
			if (v)
				ft_lstdel(&v, room_del);
			free(k);
			free(v);
		}
		free((*h)->keys);
		free((*h)->values);
		free(*h);
	}
}







static void	init_ends(t_room **r, t_htable **out, char *cur)
{
	char		**l;
	int			i;

	l = ft_strsplit(cur, ' ');
	i = 0;
	while (l[i] != NULL)
		i++;
	if (i == 3 && l[0] && l[1] && l[2] && !(*r)->r_name)
	{
		(*r)->r_name = ft_strdup(l[0]);
		(*r)->x = ft_atoi(l[1]);
		(*r)->y = ft_atoi(l[2]);
		(*r)->num_adj = 0;
		(*r)->adj = NULL;
		h_insert(out, *r);
	}
	while (i >= 0)
		free(l[i--]);
	free(l);
}

static void	init_room(t_htable **out, char *cur)
{
	char		**l;
	int			i;
	t_room		*r;

	l = ft_strsplit(cur, ' ');
	i = 0;
	while (l[i] != NULL)
		i++;
	if (i == 3 && l[0] && l[1] && l[2])
	{
		r = (t_room *)ft_memalloc(sizeof(t_room));
		r->r_name = ft_strdup(l[0]);
		r->x = ft_atoi(l[1]);
		r->y = ft_atoi(l[2]);
		r->num_adj = 0;
		r->adj = NULL;
		r->num_ants = 0;
		h_insert(out, r);
		ft_strdel(&(r->r_name));
		free(r);
	}
	while (--i >= 0)
		free(l[i]);
	free(l);
}

static void	build_ends(t_room **start, t_htable **out)
{
	char	*cur;

	if ((*start)->r_name)
		return ;
	get_next_line(0, &cur);
	printf("%s\n", cur);
	if (LEGAL(cur[0]) && ft_strchr(cur, ' ') != NULL)
		init_ends(start, out, cur);
	ft_strdel(&cur);
}

static void	build(t_room **start, t_room **end, t_htable **out)
{
	char	*cur;

	cur = NULL;
	while (get_next_line(0, &cur) && is_valid(cur, *out))
	{
		printf("%s\n", cur);
		if ((ft_strequ(cur, "##start") && (*start)->r_name)
				|| (ft_strequ(cur, "##end") && (*end)->r_name))
			break ;
		else if (ft_strequ(cur, "##start"))
			build_ends(start, out);
		else if (ft_strequ(cur, "##end"))
			build_ends(end, out);
		else if (LEGAL(cur[0]) && ft_strchr(cur, ' ') != NULL)
			init_room(out, cur);
		else if (LEGAL(cur[0]) && ft_strchr(cur, '-') != NULL)
			add_adj(cur, out);
		ft_strdel(&cur);
	}
	ft_strdel(&cur);
}

t_htable	*init_rooms(t_room **start, t_room **end, t_ant ***ants)
{
	t_htable	*out;
	char		*cur;

	cur = NULL;
	if (get_next_line(0, &cur) <= 0 || !ft_isint(cur))
	{
		//printf("init %s\n", cur);
		if (cur)
			ft_strdel(&cur);
		return (NULL);
	}
	//printf("init cur %s\n", cur);
	out = h_new();
	(*start)->num_ants = ft_atoi(cur);
	*ants = (t_ant **)ft_memalloc((*start)->num_ants * sizeof(t_ant *));
	ft_strdel(&cur);
	build(start, end, &out);
	if (out->size > 0 && (*start)->r_name && (*end)->r_name)
	{
		(*start)->num_adj = (get_room(&out, (*start)->r_name))->num_adj;
		(*end)->num_adj = (get_room(&out, (*end)->r_name))->num_adj;
	}
	if (!(*start)->r_name || !(*end)->r_name
			|| (*start)->num_adj == 0 || (*end)->num_adj == 0)
		ants_del(ants, (*start)->num_ants);
	return (out);
}





int	main(void)
{
	t_htable	*farm;
	t_room		*start;
	t_room		*end;
	t_ant		**ants;

	start = (t_room *)ft_memalloc(sizeof(t_room));
	end = (t_room *)ft_memalloc(sizeof(t_room));
	ants = NULL;
	farm = init_rooms(&start, &end, &ants);
	if (!ants)
		printf("Error: No ants or empty file\n");
	else if (farm->size == 0)
		printf("Error: No rooms\n");
	else if (!start->r_name || !end->r_name)
		printf("Error: No start or end\n");
	else if (start->num_adj == 0 || end->num_adj == 0)
		printf("Error: No valid path\n");
	else
		find_path(&ants, &farm, &start, &end);
	room_del(start, 0);
	room_del(end, 0);
	h_del(&farm);
	return (0);
}