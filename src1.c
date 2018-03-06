//GRAPH
# include <stdlib.h>
# define TRUE (1)
# define FALSE (0)

typedef int t_bool;

typedef struct		s_room
{
  int			ants;
  int			visits;
  int			distance;
  t_bool		mark;
  char			*name;
  int			x;
  int			y;
  struct s_edge		*edges;
  struct s_room		*next;
}			t_room;

typedef struct		s_edge
{
  struct s_room		*connect;
  struct s_edge		*next;
}			t_edge;

typedef struct		s_ant
{
  struct s_room		*now;
  struct s_room		*next;
}			t_ant;

typedef struct		s_anthill
{
  int			ants;
  struct s_room		*entry;
  struct s_room		*exit;
  struct s_room		*first;
}			t_anthill;

# define ERROR (1)
# define START_COMMENT "##start"
# define END_COMMENT "##end"
# define EDGE_CHAR '-'
# define SEPARATOR_CHAR ' '

typedef struct          s_line
{
  char                  *line;
  t_bool    bool;
  struct s_line         *next;
  struct s_line   *prev;
}                       t_line;

typedef struct          s_list
{
  int                   len;
  struct s_line         *first;
  struct s_line         *last;
}                       t_list;

void    printf_info(t_list *list, t_anthill *anthill);
void    free_tab(char **tab);
int recup_info(t_list *list, t_anthill *anthill);
char  *my_strdup(const char *src);
int my_strlen(const char *);
int my_printf(const char *s, ...);
int     count_tab(char **tab);
int     my_putstr_error(const char *str);
void    epur_str(char *str);
int     my_getnbr(const char *str);
int     my_strcmp(const char *s1, const char *s2);
int clear_list(t_list *list);
int     my_compare(char *s1, char *s2, int i);
int free_list(t_list *list);
int check_graph(t_anthill *anthill);
int path(t_anthill *anthill);
int move_ants(t_ant *ants, t_anthill *anthill);
int     is_end(t_ant *ants, t_anthill *anthill);
int     is_empty(t_ant *ants, t_room *room, int nb, t_anthill *anthill);
int     nb_in_next(t_ant *ants, t_room *room, int nb);
int     setup_room(char **tab, t_anthill *anthill, t_room *room);
int     create_link(t_list *list, t_anthill *anthill);
int     check_number(const char *str);
int     parse_info(t_list *list, t_anthill *anthill);


typedef struct  s_tab
{
  char          **array;
  int           nb;
  int           i;
  int           y;
  int           k;
}               t_tab;

char  **my_str_to_wordtab(char *, const char s);















static void   setup_list(t_line *new_element, t_list *list)
{
   list->len++;
   new_element->bool = FALSE;
   new_element->prev = list->last;
   new_element->next = NULL;
   if (list->last)
     list->last->next = new_element;
   else
     list->first = new_element;
   list->last = new_element;
}

int   recup_info(t_list *list, t_anthill *anthill)
{
  t_line  *new_element;
  char    *s;

  while ((s = get_next_line(0)))
    {
      if (s[0] == 0)
  return (ERROR);
      epur_str(s);
      if (!(new_element = malloc(sizeof(*new_element))) ||
    (!(new_element->line = my_strdup(s))))
  return (ERROR);
      setup_list(new_element, list);
      free(s);
    }
  clear_list(list);
  if (list->first == NULL)
    return (ERROR);
  return (parse_info(list, anthill));
}


static t_ant    *get_ants(int nb, t_anthill *anthill)
{
  t_ant         *ants;
  int           i;

  if (!(ants = malloc(sizeof(*ants) * nb)))
    return (NULL);
  i = 0;
  while (i < nb)
    {
      ants[i].now = anthill->entry;
      ants[i].next = NULL;
      i += 1;
    }
  return (ants);
}

void  init(t_anthill *anthill, t_list *list)
{
  anthill->first = NULL;
  anthill->exit = NULL;
  anthill->entry = NULL;
  list->first = NULL;
  list->last = NULL;
  list->len = 0;
}

int   main(int ac, char **av)
{
  t_list  list;
  t_anthill anthill;
  t_ant   *ants;

  if (ac != 1 || av[1] != NULL)
    return (my_printf("USAGE:\n\t ./lem_in < map\n"));
  init(&anthill, &list);
  if (recup_info(&list, &anthill) == ERROR ||
      anthill.entry == NULL || anthill.exit == NULL)
    {
      printf_info(&list, &anthill);
      return (ERROR);
    }
  anthill.entry->ants = anthill.ants;
  check_graph(&anthill);
  if (anthill.entry->mark == FALSE)
    {
      printf_info(&list, &anthill);
      return (ERROR);
    }
  printf_info(&list, &anthill);
  if (!(ants = get_ants(anthill.entry->ants, &anthill)))
    return (1);
  move_ants(ants, &anthill);
  return (0);
}









