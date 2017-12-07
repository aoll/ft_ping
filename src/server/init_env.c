/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/29 03:12:04 by alex              #+#    #+#             */
/*   Updated: 2017/11/29 08:26:09 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static void		new_client(t_env *e, int cs)
{
	clean_fd(&e->fds[cs]);
	e->fds[cs].type = FD_CLIENT;
	e->fds[cs].fct_read = client_read;
	e->fds[cs].fct_write = client_write;
	if (!(e->fds[cs].buf_read = ft_buf_new(BUF_SIZE)))
	{
		printf("%s\n", "ERROR: new_client");
		exit(EXIT_FAILURE);
	}
	if (!(e->fds[cs].buf_write = ft_buf_new(BUF_SIZE)))
	{
		printf("%s\n", "ERROR: new_client");
		exit(EXIT_FAILURE);
	}
	ft_bzero(e->fds[cs].name, MAX_LEN_LOGIN_NAME);
	ft_strcpy(e->fds[cs].name, GUEST);
	ft_strcpy(e->fds[cs].name + ft_strlen(GUEST), ft_itoa(cs));
	ft_bzero(e->fds[cs].channel, MAX_LEN_CHANEL_NAME);
}

static void		srv_accept(t_env *e, int s)
{
	int			cs;
	struct sockaddr_in	csin;
	socklen_t		csin_len;

	csin_len = sizeof(csin);
	if ((cs = accept(s, (struct sockaddr*)&csin, &csin_len)) == -1)
	{
		printf("%s\n", "ERROR ACCEPT");
		exit (1);
	}
	printf("New client #%d from %s:%d\n", cs,
		inet_ntoa(csin.sin_addr), ntohs(csin.sin_port));
	new_client(e, cs);
}

static int	init_env_fd(t_env *e)
{
	int		i;
	struct rlimit	rlp;

	if (getrlimit(RLIMIT_NOFILE, &rlp))
		return (EXIT_FAILURE);
	e->maxfd = rlp.rlim_cur;
	if (!(e->fds = malloc(sizeof(*e->fds) * e->maxfd)))
		return (EXIT_FAILURE);
	i = 0;
	while (++i < e->maxfd)
	{
		clean_fd(&e->fds[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}

int			init_env_channel(t_env *e)
{
	if (!(e->channels = ft_chanels_init(NB_MAX_CHANNEL)))
	{
		return (EXIT_FAILLURE);
	}
	return (EXIT_SUCCESS);
}

int			init_env(t_env *e, int port)
{
	int sock;

	if (init_env_fd(e))
	{
		printf("%s\n", "ERROR: init env_fd");
		return (EXIT_FAILLURE);
	}
	if (init_env_channel(e))
	{
		printf("%s\n", "ERROR: init_env_channel");
		return (EXIT_FAILLURE);
	}
	if ((sock = create_server(port, e->maxfd)) < 0)
	{
		printf("%s\n", "ERROR: create_server");
		return (EXIT_FAILLURE);
	}
	e->fds[sock].type = FD_SERV;
	e->fds[sock].fct_read = srv_accept;
	return (EXIT_SUCCESS);
}
