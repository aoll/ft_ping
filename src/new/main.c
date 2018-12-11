/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 16:58:49 by alex              #+#    #+#             */
/*   Updated: 2018/12/11 06:01:25 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void intHandler(int dummy)
{
	g_is_stop = 1;
}

float					ft_sqrtl(float x)
{
	float n;

	n = x / 2;
	while (ABS(n * n - x) > TOLERANCE)
	{
		n = (n + x / n) / 2;
	}
	return n;
}

int	init_socket(void)
{
	int s = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);	/* open raw socket */

	const int val=MY_TTL;
	if ( setsockopt(s, SOL_IP, IP_TTL, &val, sizeof(val)) != 0) {
		printf("%s\n", "ERROR");
		exit(EXIT_FAILURE);
	}

	return (s);
}

struct sockaddr	*get_addr(const char *adr, t_env *e)
{
	struct addrinfo	*res;
	struct addrinfo	hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags                = 0;
	hints.ai_family               = AF_INET;
	hints.ai_socktype             = SOCK_RAW;
	hints.ai_protocol             = IPPROTO_ICMP;
	if (getaddrinfo(adr, 0, &hints, &res) < 0)
	{
		printf("ft_ping: unknown host %s\n", adr);
		exit(EXIT_FAILURE);
	}
	if (!inet_ntop(AF_INET, &((struct sockaddr_in *)res->ai_addr)->sin_addr,
		e->ipv4, INET_ADDRSTRLEN))
	{
		printf("Error to get adress.\n");
		exit(EXIT_FAILURE);
	}
	return (res->ai_addr);
}


void	usage(void)
{
	printf("%s\n", "Usage: ft_ping [-hv] [-h help] [-v verbose] destination");
	exit(EXIT_FAILURE);
	return ;
}

static void	display_nb_packets(t_env *e)
{
	printf("%d packets transmitted, ", e->nb_packet_send);
	printf("%d received, ", e->nb_packet_rcv);
	if (e->nb_packet_error)
		printf("+%d errors, ", e->nb_packet_error);
	printf("%d%s packet loss, ",
	e->nb_packet_rcv != e->nb_packet_send ?
		e->nb_packet_rcv / e->nb_packet_send * 100 : 0, "%");
	printf("time %dms\n", e->nb_packet_error);
	return ;
}

static void		display_statistics(t_env *e)
{
	printf("\n--- %s ft_ping statistics ---\n", e->adr);
	display_nb_packets(e);
	printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
		(e->min), (e->total/e->nb_packet_rcv),(e->max),
		ft_sqrtl((e->tsum/(e->nb_packet_rcv + e->nb_packet_send)) -
		(e->total/(e->nb_packet_rcv + e->nb_packet_send)) *
		(e->total/(e->nb_packet_rcv + e->nb_packet_send))));
	return ;
}

int	loop(t_env *e)
{
	struct timeval t_time;
	time_t time_last_send;

	time_last_send = 0;
	init_packet(e);
	while (42)
	{
		gettimeofday(&t_time, NULL);
		if (t_time.tv_sec >= time_last_send + 1)
		{
			time_last_send = t_time.tv_sec;
			send_packet(e);
		}
		read_packet(e);
		if (g_is_stop)
			break;
	}
	display_statistics(e);
	return (EXIT_SUCCESS);
}

void	check(int ac, char **av, t_env *e)
{
	int i;
	int j;
	int nb_dest;

	i = 0;
	nb_dest = 0;
	while (++i < ac)
		if (av[i][0] == '-')
		{
			if (!av[i][1])
				usage();
			j = 0;
			while (av[i][++j])
				if (av[i][j] == 'h')
					usage();
				else if (av[i][j] == 'v')
					e->option_v = 1;
		}
		else
		{
			e->adr = av[i];
			nb_dest++;
		}
	if (ac < 2 || nb_dest != 1)
		usage();
}

void	init_env(int ac, char **av, t_env *e)
{
	gettimeofday(&e->start_time, NULL);
	e->pid = getpid();
	e->seq = 1;
	e->option_t = 0;
	e->option_v = 0;
	e->nb_packet_send = 0;
	e->nb_packet_rcv = 0;
	e->nb_packet_error = 0;
	e->min = 0;
	e->max = 0;
	e->total = 0;
	ft_bzero(e->ipv4, INET_ADDRSTRLEN);
	check(ac, av, e);
	e->socket = init_socket();
	e->ad_dst = get_addr(e->adr, e);
	return ;
}

int
main (int ac, char **av)
{
	struct sockaddr *ad_dst;
	t_env e;

	if (getuid())
	{
		printf("%s\n", "Must be root.");
		return (EXIT_FAILURE);
	}
	g_is_stop = 0;
	init_env(ac, av, &e);
	signal(SIGINT, intHandler);
	printf("PING %s (%s) 56(84) bytes of data.\n", e.adr, e.ipv4);
	return (loop(&e));
}
