/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_server.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/29 01:28:09 by alex              #+#    #+#             */
/*   Updated: 2017/11/29 01:31:04 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	create_server(int port, int nb_conn)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in6	sin;

	if (!(proto = getprotobyname(PROTOCOLE)))
		return (-1);
	ft_memset(&sin, 0, sizeof(sin));
	if ((sock = socket(PF_INET6, SOCK_STREAM, proto->p_proto)) == -1)
		return (-1);
	sin.sin6_family = AF_INET6;
	sin.sin6_port = htons(port);
	sin.sin6_addr = in6addr_any;
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		close (sock);
		return (-1);
	}
	if (listen(sock, nb_conn) == -1)
	{
		close (sock);
		return (-1);
	}
	return (sock);
}
