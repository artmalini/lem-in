/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 11:33:12 by amakhiny          #+#    #+#             */
/*   Updated: 2017/12/12 11:33:29 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../includes/get_next_line.h"

// static	t_list	*ft_correct_fd(t_list **head, size_t fd)
// {
// 	t_list		*node;

// 	node = *head;
// 	if (node)
// 	{
// 		while (node)
// 		{
// 			if (fd == node->content_size)
// 				return (node);
// 			node = node->next;
// 		}
// 	}
// 	node = ft_lstnew("\0", 1);
// 	node->content_size = fd;
// 	ft_lstadd(head, node);
// 	return (node);
// }

// static	char	*ft_strndup(char *sorc, size_t n)
// {
// 	char		*dup;

// 	if (!(dup = (char *)malloc(sizeof(char) * n + 1)))
// 		return (NULL);
// 	dup = ft_strncpy(dup, sorc, n);
// 	dup[n] = '\0';
// 	return (dup);
// }

// static	char	*ft_strnjoin(char const *s1, char const *s2, size_t len)
// {
// 	char		*s3;

// 	if (!s1 || !s2)
// 		return (NULL);
// 	if (!(s3 = ft_strnew(ft_strlen(s1) + len)))
// 		return (NULL);
// 	s3 = ft_strcpy(s3, s1);
// 	s3 = ft_strncat(s3, s2, len);
// 	return (s3);
// }

// static	char	*ft_add(char *content, char *buff, int result)
// {
// 	char		*p;

// 	p = content;
// 	content = ft_strnjoin(content, buff, result);
// 	free(p);
// 	return (content);
// }

// int				get_next_line(const int fd, char **line)
// {
// 	static	t_list	*new_line;
// 	t_list			*head;
// 	char			buff[BUFF_SIZE + 1];
// 	int				result;
// 	char			*s;

// 	if (fd < 0 || read(fd, buff, 0) < 0 || !line)
// 		return (-1);
// 	head = new_line;
// 	new_line = ft_correct_fd(&head, fd);
// 	while (!ft_strchr(new_line->content, '\n') &&
// 		   (result = read(fd, buff, BUFF_SIZE)))
// 		new_line->content = ft_add(new_line->content, buff, result);
// 	result = 0;
// 	while (((char *)new_line->content)[result] &&
// 		   ((char *)new_line->content)[result] != '\n')
// 		result++;
// 	*line = ft_strndup(new_line->content, result);
// 	if (((char *)new_line->content)[result] == '\n')
// 		result++;
// 	s = new_line->content;
// 	new_line->content = ft_strdup(new_line->content + result);
// 	free(s);
// 	new_line = head;
// 	return (result ? 1 : 0);
// }
static t_mem	*readbuff(t_mem *lst, const int fd)
{
	t_mem			*elem;

	elem = lst;
	while (elem)
	{
		if (elem->fd == fd)
			break ;
		elem = elem->next;
	}
	if (!elem)
	{
		if (!(elem = (t_mem *)malloc(sizeof(t_mem))))
			return (0);
		if (!(elem->content = ft_strnew(0)))
			return (0);
		elem->fd = fd;
		elem->next = NULL;
		while (lst && lst->next)
			lst = lst->next;
		if (lst)
			lst->next = elem;
	}
	return (elem);
}

static int		get_line(t_mem *lst, char **line)
{
	char	*temp;
	char	*ret;

	if (!(temp = ft_strdup(lst->content)))
		return (-1);
	free(lst->content);
	if (*temp == '\n')
	{
		if (!(*line = ft_strnew(0)) ||
			!(lst->content = ft_strdup(temp + 1)))
			return (-1);
		free(temp);
		return (1);
	}
	if ((ret = ft_strchr(temp, '\n')))
	{
		if (!(lst->content = ft_strdup(ret + 1)))
			return (-1);
		*ret = '\0';
	}
	else
		lst->content = ft_strnew(0);
	ret = temp;
	*line = ret;
	return (!*ret ? 0 : 1);
}

int				get_next_line(const int fd, char **line)
{
	static t_mem	*mem;
	t_mem			*lst;
	int				val;
	char			*box;
	char			buff[BUFF_SIZE + 1];

	if (fd < 0 || !line || !(lst = readbuff(mem, fd)) || BUFF_SIZE < 1)
		return (-1);
	if (!mem)
		mem = lst;
	while ((val = read(fd, buff, BUFF_SIZE)) > 0)
	{
		buff[val] = '\0';
		if (!(box = ft_strjoin(lst->content, buff)))
			return (-1);
		free(lst->content);
		lst->content = box;
		if (ft_strchr(lst->content, '\n'))
			return (get_line(lst, line));
	}
	if (val == -1)
		return (-1);
	return (get_line(lst, line));
}
