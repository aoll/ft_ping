/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/29 03:15:21 by alex              #+#    #+#             */
/*   Updated: 2017/12/04 12:38:10 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	send_dada(t_env *e, int cs, char *buf, int r)
{
	int i;

	printf("read: %d, %s\n",r, buf);
	i = -1;
	while (++i < e->maxfd)
	{
		if ((e->fds[i].type == FD_CLIENT) &&
			(i != cs))
		{
			if (ft_buf_add_data(
				e->fds[i].buf_write, e->fds[cs].name, ft_strlen(e->fds[cs].name)))
			{
				ft_buf_clean(e->fds[i].buf_write);
				printf("%s\n", "ERROR: too much data");
				return ;
			}
			if (ft_buf_add_data(e->fds[i].buf_write, ": ", 2))
			{
				ft_buf_clean(e->fds[i].buf_write);
				printf("%s\n", "ERROR: too much data");
				return ;
			}
			if (ft_buf_add_data(e->fds[i].buf_write, buf, r))
			{
				ft_buf_clean(e->fds[i].buf_write);
				printf("%s\n", "ERROR: too much data");
				return ;
			}
		}
	}
	return ;
}

int	switch_requete(t_env *e, int cs, char *command, int cmdsize)
{
	char *cmd;

	if (!(cmd = ft_strtrim(command)))
		exit(EXIT_FAILLURE);
	if (!ft_strncmp(cmd, NICKNAME, ft_strlen(NICKNAME)))
	{
		set_name(e, cs, cmd);
	}
	else if (!ft_strncmp(cmd, NEW_CHANNEL, ft_strlen(NEW_CHANNEL)))
	{
		new_channel(e, cs, cmd);
	}
	else if (!ft_strncmp(cmd, JOIN_CHANNEL, ft_strlen(JOIN_CHANNEL)))
	{
		join_channel(e, cs, cmd);
	}
	else if (!ft_strncmp(cmd, LEAVE_CHANNEL, ft_strlen(LEAVE_CHANNEL)))
	{
		leave_channel(e, cs, cmd);
	}
	else if (!ft_strncmp(cmd, WHO, ft_strlen(WHO)))
	{
		who(e, cs, cmd);
	}
	else if (!ft_strncmp(cmd, MSG, ft_strlen(MSG)))
	{
		message(e, cs, cmd);
	}
	// else
	// {
	// 	send_dada(e, cs, cmd, cmdsize);
	// }
	return (EXIT_SUCCESS);
}

void	recv_data(t_env *e, int cs, char *buf, int r)
{
	char data[BUF_SIZE + 1];
	int	buf_size;

	printf("BUF: %s\n", buf);
	ft_bzero(data, BUF_SIZE + 1);
	if (ft_buf_add_data(e->fds[cs].buf_read, buf, r))
	{
		ft_buf_clean(e->fds[cs].buf_read);
		printf("%s\n", "ERROR: too much data");
		return ;
	}
	if (e->fds[cs].buf_read->start_data == e->fds[cs].buf_read->end_data
		|| buf[r - 1] == 10)
	{
		buf_size = ft_buf_get_data(e->fds[cs].buf_read, data);
		if (data[buf_size - 1] != 10)
		{
			ft_buf_clean(e->fds[cs].buf_read);
			printf("%s\n", "ERROR: too much data");
			return ;
		}
		printf("buf_size: %d\n", buf_size);
		switch_requete(e, cs, data, buf_size);
		// send_dada(e, cs, data, buf_size);
	}
	return ;
}

void	client_read(t_env *e, int cs)
{
	int	r;
	char buf[BUF_SIZE + 1];

	ft_bzero(buf, BUF_SIZE + 1);
	r = recv(cs, buf, BUF_SIZE, 0);
	if (r <= 0)
	{
		close(cs);
		clean_fd(&e->fds[cs]);
		printf("client #%d gone away\n", cs);
	}
	else
	{
		recv_data(e, cs, buf, r);
	}
}
