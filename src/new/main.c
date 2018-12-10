/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 16:58:49 by alex              #+#    #+#             */
/*   Updated: 2018/12/10 01:30:36 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"




int g_is_stop = 0;

void intHandler(int dummy)
{
	// printf("%s\n", "intHandler");
	g_is_stop = 1;
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

struct sockaddr	*get_addr(const char *adr)
{
	struct addrinfo *res;
	struct addrinfo  hints;  //prefered addr type(connection)


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
	printf("%s\n", "SUCCESS");
	return (res->ai_addr);

}


void	usage(void)
{
	printf("%s\n", "Usage: ft_ping [-hv] [-h help] [-v verbose] destination");
	exit(EXIT_FAILURE);
	return ;
}

int	loop(t_env *e)
{
	// int sequenceno = 1;
	struct timeval t_time;
	time_t time_last_send = 0;

	while (42)
	{
		if (gettimeofday(&t_time, NULL))
		{
			printf("%s\n", "ERROR: gettimeofday" );
			exit (EXIT_FAILURE);
		}
		if (t_time.tv_sec >= time_last_send + 1)
		{
			time_last_send = t_time.tv_sec;
			send_packet(e);
			e->seq++;
		}
		read_packet(e);
		if (g_is_stop)
		{
			break;
		}
	}
	close(e->socket);
	printf("\n--- %s ft_ping statistics ---\n", e->adr);
	return (EXIT_SUCCESS);
}

void	check(int ac, char **av, t_env *e)
{
	int i = 1;
	int j;
	int nb_dest = 0;

	if (ac < 2)
		usage();
	while (i < ac)
	{
		if (av[i][0] == '-')
		{
			if (!av[i][1])
				usage();
			j = 1;
			while (av[i][j])
			{
				if (av[i][j] == 'h')
					usage();
				else if (av[i][j] == 'v')
					e->option_v = 1;
				j++;
			}
		}
		else
		{
			e->adr = av[i];
			nb_dest++;
		}
		i++;
	}
	if (nb_dest != 1)
		usage();
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

	e.pid = getpid();
	e.seq = 1;
	e.option_t = 0;
	e.option_v = 0;
	e.nb_packet_send = 0;
	e.nb_packet_rcv = 0;
	e.nb_packet_error = 0;
	check(ac, av, &e);



	e.socket = init_socket();


	e.ad_dst = get_addr(e.adr);

	signal(SIGINT, intHandler);//catching interrupt
	loop(&e);

	return (EXIT_SUCCESS);
}
