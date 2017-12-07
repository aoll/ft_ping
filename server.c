#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "server.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


/*
** usage
*/

void	usage(char *s)
{
	printf("Usage: %s <port>\n", s);
	exit(EXIT_FAILLURE);
}

/*
** TCP/IP (v4) server example from 42 school
*/

int	main(int ac, char **av)
{
	int					port;

	if (ac != 2)
		usage(av[0]);
	if ((port = atoi(av[1])) <= 0)
		usage(av[0]);
	start_server(port);
	return (EXIT_SUCCESS);
}
