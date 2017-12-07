/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_server.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/29 01:47:21 by alex              #+#    #+#             */
/*   Updated: 2017/12/03 18:24:15 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	check_fd(t_env *e)
{
	int	i;

	i = 0;
	while ((i < e->maxfd) && (e->r > 0))
	{
		if (FD_ISSET(i, &e->fd_read))
			e->fds[i].fct_read(e, i);
		if (FD_ISSET(i, &e->fd_write))
			e->fds[i].fct_write(e, i);
		if (FD_ISSET(i, &e->fd_read) ||
			FD_ISSET(i, &e->fd_write))
			e->r--;
		i++;
	}
}

void	init_fd(t_env *e)
{
	int	i;

	i = 0;
	e->max = 0;
	FD_ZERO(&e->fd_read);
	FD_ZERO(&e->fd_write);
	while (i < e->maxfd)
	{
		if (e->fds[i].type != FD_FREE)
		{
			FD_SET(i, &e->fd_read);
			if (e->fds[i].type == FD_CLIENT)
			{
				if (e->fds[i].buf_write->size > 0)
				{
					if (*(e->fds[i].buf_write->end_data - 1) == 10 ||
					(e->fds[i].buf_write->end_data ==
						e->fds[i].buf_write->start_buf &&
					*(e->fds[i].buf_write->end_buf - 1) == 10))
					{
						FD_SET(i, &e->fd_write);
					}
				}
			}
			e->max = MAX(e->max, i);
		}
		i++;
	}
}

int	server_listen(t_env *e)
{
	while (42)
	{
		init_fd(e);
		printf("%s\n", "start loop");
		e->r = select(e->max + 1, &e->fd_read, &e->fd_write, NULL, NULL);
		if (!e->r)
			continue ;
		if (e->r < 0)
		{
			printf("%s\n", "ERROR");
			exit(EXIT_FAILLURE);
		}
		check_fd(e);
	}
	return (EXIT_SUCCESS);
}

int	start_server(int port)
{
	t_env				e;

	if (init_env(&e, port))
	{
		printf("%s\n", "ERROR: init env_fd");
		return (EXIT_FAILLURE);
	}
	printf("%s\n", "Yo!");
	server_listen(&e);
	// close(cs);
	// close(3);
	return (EXIT_SUCCESS);
}
