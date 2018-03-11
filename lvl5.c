#include <stdlib.h>
#include <limits.h>

int		ft_strlen(char *str)
{
	int		i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

int		ft_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int		is_operateur(char *str)
{
	int		i;

	i = 0;
	if (str[i] == '*' || str[i] == '+' || str[i] == '-' || str[i] == '%' || str[i] == '/')
	{
		if (ft_isdigit(str[i + 1]) == 0)
			return (1);
	}
	return (0);
}

long		*rpn_calc(char *str)
{
	long	*tab;
	int		i;
	int		j;
		
	i = 0;
	j = 0;
	if (!(tab = (long*)malloc(sizeof(long) * ft_strlen(str))))
		return (NULL);
	while (str[i] != '\0')
	{
		while (is_operateur(str + i) == 0)
		{
			tab[j] = atoi(str + i);
			j++;
			while (str[i] != '\0' && str[i] != ' ')
				i++;
			if (str[i] == '\0')
			{
				printf("Error\n");
				return (NULL);
			}
			while (str[i] == ' ')
				i++;
		}
		if (j < 2)
		{
			printf("Error\n");
			return (NULL);
		}
		if (str[i] == '/')
		{
			if (tab[j - 1] == 0)
			{
				printf("Error\n");
				return (NULL);
			}
			tab[j - 2] = tab[j - 2] / tab[j - 1];
		}
		else if (str[i] == '-')
			tab[j - 2] = tab[j - 2] - tab[j - 1];
		else if (str[i] == '+')
			tab[j - 2] = tab[j - 2] + tab[j - 1];
		else if (str[i] == '*')
			tab[j - 2] = tab[j - 2] * tab[j - 1];
		else if (str[i] == '%')
		{
			if (tab[j - 1] == 0)
			{
				printf("Error\n");
				return (NULL);
			}
			tab[j - 2] = tab[j - 2] % tab[j - 1];
		}
		j--;
		i++;
		while (str[i] == ' ')
			i++;
	}
	if (j > 1)
	{
		printf("Error\n");
		return (NULL);
	}
	return (tab);
}

int		main(int argc, char **argv)
{
	long	*tab;

	if (argc == 2 && argv[1][0] != '\0')
	{
		tab = rpn_calc(argv[1]);
		if (tab != NULL)
			printf("%ld\n", tab[0]);
		return (0);
	}
	printf("Error\n");
	return (0);
}







//brackets
//#include <unistd.h>

int	braclose(char *str, char c, int i, int b)
{
	while (b && *(++str) && (i++))
		if (*str == c || *str == c + c % 2 + 1)
			*str == c ? ++b : --b;
	return (i);
}

int	brackets(char *str, char c)
{
	if (*str == c)
		return (1);
	else if (!*str || *str == ')' || *str == '}' || *str == ']')
		return (0);
	else if (*str == '(' || *str == '{' || *str == '[')
		return (brackets(str + 1, *str + *str % 2 + 1) * brackets(str + braclose(str, *str, 1, 1), c));
	else
		return (brackets(str + 1, c));
}

int	main(int ac, char **av)
{
	int	i;

	i = 0;
	if (ac > 1)
		while (++i < ac)
			brackets(av[i], 0) ? write(1, "OK\n", 3) : write(1, "Error\n", 6);
	else
		write(1, "\n", 1);
	return (0);
}