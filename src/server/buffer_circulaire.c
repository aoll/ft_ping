/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_circulaire.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 03:54:07 by alex              #+#    #+#             */
/*   Updated: 2017/12/01 01:41:44 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	ft_buf_set_data(t_buf *buf, const char *data, size_t size, char *err_mess)
{
	if (ft_buf_add_data(buf, data, size))
	{
		ft_buf_clean(buf);
		printf("%s\n", err_mess);
		buf_send_error(buf, INTERN_ERROR, ft_strlen(INTERN_ERROR));
		return (EXIT_FAILLURE);
	}
	return (EXIT_SUCCESS);
}

void	ft_buf_clean(t_buf *buf_e)
{
	buf_e->size = 0;
	buf_e->start_data = buf_e->start_buf;
	buf_e->end_data = buf_e->start_buf;
	return;
}

t_buf		*ft_buf_new(size_t size)
{
	t_buf	*buf_e;

	if (!(buf_e = malloc(size + sizeof(*buf_e))))
		return (NULL);

	buf_e->size = 0;
	buf_e->start_buf = (void *)buf_e + sizeof(*buf_e);
	buf_e->end_buf = (void *)buf_e->start_buf + size;
	buf_e->start_data = buf_e->start_buf;
	buf_e->end_data = buf_e->start_buf;
	return (buf_e);
}

static int	ft_buf_add_data_to_end(const char *s, size_t size, t_buf *buf_e)
{
	ft_strncpy(buf_e->end_data, s, size);
	buf_e->end_data += size;
	buf_e->size += size;
	if (buf_e->end_data == buf_e->end_buf)
		buf_e->end_data = buf_e->start_buf;
	return (EXIT_SUCCESS);
}

static int	ft_buf_add_data_start_end(const char *s, size_t size, t_buf *buf_e)
{
	long size_to_end_buf;
	long size_from_begin;

	size_to_end_buf = buf_e->end_buf - buf_e->end_data;
	size_from_begin = size - size_to_end_buf;
	if (size_from_begin > 0)
	{
		if (buf_e->start_buf + size_from_begin > buf_e->start_data)
			return (EXIT_FAILURE);
		ft_strncpy(buf_e->end_data, s, size_to_end_buf);
		ft_strncpy(buf_e->start_buf, s, size_from_begin);
		buf_e->end_data = buf_e->start_buf + size_from_begin;
		buf_e->size += size;
		return (EXIT_SUCCESS);
	}
	else
	{
		return (ft_buf_add_data_to_end(s, size, buf_e));
	}
}

int			ft_buf_add_data(t_buf *buf_e, const char *s, size_t size)
{
	if (buf_e->start_buf + size > buf_e->end_buf)
		return (EXIT_FAILURE);
	if (buf_e->end_data == buf_e->start_data)
		return (ft_buf_add_data_to_end(s, size, buf_e));
	if (buf_e->end_data > buf_e->start_data)
		return (ft_buf_add_data_start_end(s, size, buf_e));
	if (buf_e->end_data + size > buf_e->start_data)
		return (EXIT_FAILURE);
	return (ft_buf_add_data_to_end(s, size, buf_e));
}

int		ft_buf_get_data(t_buf *buf_e, char *dest)
{
	int	i;

	i = 0;
	if (buf_e->size == (size_t)(buf_e->end_buf - buf_e->start_buf))
	{
		*dest = *buf_e->start_data;
		dest++;
		buf_e->start_data++;
		i++;
	}
	if (buf_e->start_data == buf_e->end_buf)
		buf_e->start_data = buf_e->start_buf;
	while (buf_e->start_data != buf_e->end_data)
	{
		*dest = *buf_e->start_data;
		dest++;
		buf_e->start_data++;
		if (buf_e->start_data == buf_e->end_buf)
			buf_e->start_data = buf_e->start_buf;
		i++;
	}
	buf_e->size = 0;
	return (i);
}
