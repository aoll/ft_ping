/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_write.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/29 03:18:45 by alex              #+#    #+#             */
/*   Updated: 2017/11/29 05:32:12 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	client_write(t_env *e, int cs)
{
	char buf[BUF_SIZE + 1];
	int	buf_size;

	ft_bzero(buf, BUF_SIZE + 1);
	buf_size = ft_buf_get_data(e->fds[cs].buf_write, buf);
	if (buf[buf_size - 1] != 10)
		return ;
	send(cs, buf, buf_size, 0);
	return ;
}
