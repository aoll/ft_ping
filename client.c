/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/04 12:54:52 by alex              #+#    #+#             */
/*   Updated: 2017/12/04 14:48:34 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	usage(const char *name)
{
	printf("Usage %s <machine> <port> || [machine [port]]\n", name);
	return ;
}

int	main(int ac, char **av)
{
	if (ac < 2 || ac > 3)
		usage(av[0]);
	start_client(av[1], av[2]);
	return (EXIT_SUCCESS);
}
