/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_client.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/04 13:23:45 by alex              #+#    #+#             */
/*   Updated: 2017/12/04 15:24:42 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

int	init_fd_client(fd_set *fd_read, fd_set *fd_write, int sock, int stdin)
{
	FD_ZERO(fd_read);
	FD_ZERO(fd_write);
	FD_SET(sock, fd_read);
	FD_SET(stdin, fd_read);
	// FD_SET(sock, fd_write);
	return (EXIT_SUCCESS);
}

int	read_sock_stdin(int fd, int sock_ref, char *buf)
{
	int r;

	r = read(fd, buf, BUF_SIZE);
	if (r <= 0)
	{
		close(fd);
		exit (EXIT_SUCCESS);
	}
	if (!ft_strncmp(buf, "/connect", 8))
	{
		return (2);
	}
	return (EXIT_SUCCESS);
}

int	read_sock_sock(int fd, int sock_ref)
{
	int	r;
	char buf[BUF_SIZE + 1];

	ft_bzero(buf, BUF_SIZE + 1);
	r = read(fd, buf, BUF_SIZE);
	if (r <= 0)
	{
		close(fd);
		exit (EXIT_SUCCESS);
	}
	printf("%s", buf);
	return (EXIT_SUCCESS);
}

int	write_sock(int sock, char *buf, size_t size)
{
	if (send(sock, buf, size, 0) == -1)
		exit(EXIT_FAILURE);
	ft_bzero(buf, BUF_SIZE + 1);
	return (EXIT_SUCCESS);
}

int	new_connection(int sock, char *buf)
{
	char	addr[BUF_SIZE];
	char	port[BUF_SIZE];
	char	**split;

	if (!(split = ft_strsplit(buf + 8, ' ')))
		exit (EXIT_FAILURE);
	ft_strcpy(addr, split[0]);
	ft_strcpy(port, split[1]);
	if (sock)
	{
		close(sock);
	}
	ft_array_free(&split);
	return (start_client(addr, port));
}

int	listen_client(int sock, int stdin)
{
	fd_set		fd_read;
	fd_set		fd_write;
	int			r;
	char 		buf[BUF_SIZE + 1];

	ft_bzero(buf, BUF_SIZE + 1);
	while (42)
	{
		init_fd_client(&fd_read, &fd_write, sock, stdin);
		if (ft_strlen(buf))
			FD_SET(sock, &fd_write);
		r = select(sock + 1, &fd_read, &fd_write, NULL, NULL);
		if (!r)
			continue ;
		if (FD_ISSET(sock, &fd_read))
			read_sock_sock(sock, sock);
		if (FD_ISSET(stdin, &fd_read))
			if (read_sock_stdin(stdin, sock, buf) == 2)
				return (new_connection(sock, buf));
		if (FD_ISSET(sock, &fd_write) && ft_strlen(buf))
			write_sock(sock, buf, ft_strlen(buf));
	}
}

int	start_client(char *addr, char *port)
{
	int		sock;
	char	*trim_addr;
	char	*trim_port;

	if (!(trim_addr = ft_strtrim(addr)))
		exit (EXIT_FAILURE);
	if (!(trim_port = ft_strtrim(port)))
	{
		free(trim_addr);
		exit (EXIT_FAILURE);
	}
	if (!(sock = create_client(trim_addr, trim_port)))
		return (EXIT_FAILURE);
	free(trim_addr);
	free(trim_port);
	listen_client(sock, STDIN);
	return (EXIT_SUCCESS);
}
