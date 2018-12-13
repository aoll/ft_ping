/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 00:51:09 by alex              #+#    #+#             */
/*   Updated: 2018/12/12 23:52:10 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static int	process_packet(unsigned char buf[], int bytes, t_env *e)
{
	if (
		check_type(
			(struct icmphdr *)((void *)buf + sizeof(struct ip)), bytes, e)
		|| check_size(bytes, e)
		|| check_pid((struct icmphdr *)((void *)buf + sizeof(struct ip)), e)
		|| check_sum((struct icmphdr *)((void *)buf + sizeof(struct ip)), e)
		|| check_data((char *)buf + (PACKETSIZE_TOTAL - MY_DATA_LEN), e))
	{
		e->nb_packet_error++;
		return (EXIT_FAILURE);
	}
	display((struct icmphdr *)((void *)buf + sizeof(struct ip)),
	(struct timeval *)(
		(void *)buf + sizeof(struct ip) + sizeof(struct icmphdr)),
		(struct ip *)buf, e);
	e->nb_packet_rcv++;
	return (EXIT_SUCCESS);
}

int						read_packet(t_env *e)
{
	struct msghdr	msg;
	struct iovec	iov;
	unsigned char	iov_base[PACKETSIZE_TOTAL];
	ssize_t			ret;

	memset(&msg, 0, sizeof(msg));
	memset(&iov, 0, sizeof(iov));
	msg.msg_name = e->res->ai_addr;
	msg.msg_namelen = e->res->ai_addrlen;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	iov.iov_base = &iov_base;
	iov.iov_len = PACKETSIZE_TOTAL;
	ret = recvmsg(e->socket, &msg, MSG_DONTWAIT);

	if (ret > 0)
	{
		return (process_packet(iov_base, ret, e));
	}
	return (EXIT_SUCCESS);
}
