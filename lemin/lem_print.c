#include "includes/lem_in.h"

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
	return ;
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
