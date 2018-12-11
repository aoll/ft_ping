/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 16:58:49 by alex              #+#    #+#             */
/*   Updated: 2018/12/11 07:43:09 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

int			loop(t_env *e)
{
	struct timeval	t_time;
	time_t			time_last_send;

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
			break ;
	}
	display_statistics(e);
	return (EXIT_SUCCESS);
}

void		check(int ac, char **av, t_env *e)
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

int			main(int ac, char **av)
{
	struct sockaddr	*ad_dst;
	t_env			e;

	if (getuid())
	{
		printf("%s\n", "Must be root.");
		return (EXIT_FAILURE);
	}
	g_is_stop = 0;
	init_env(ac, av, &e);
	signal(SIGINT, int_handler);
	printf("PING %s (%s) 56(84) bytes of data.\n", e.adr, e.ipv4);
	return (loop(&e));
}
