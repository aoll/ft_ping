/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 03:01:58 by alex              #+#    #+#             */
/*   Updated: 2017/12/03 17:37:42 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static int	get_name(char *cmd, char *name)
{
	int		i;
	int		j;

	i = ft_strlen(MSG);
	while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '\t' || cmd[i] == '\n'))
		i++;
	j = 0;
	if (!cmd[i])
		return (-1);
	while (cmd[i] && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '\n')
	{
		if (j >= MAX_LEN_LOGIN_NAME)
			return (-1);
		name[j] = cmd[i];
		i++;
		j++;
	}
	while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '\t' || cmd[i] == '\n'))
		i++;
	return (i);
}

static int	send_dada(t_env *e, int cs, char *buf, char *target_name)
{
	int i;

	i = -1;
	while (++i < e->maxfd)
	{
		if ((e->fds[i].type == FD_CLIENT) &&
			(i != cs) && !ft_strcmp(e->fds[i].name, target_name))
		{
			if (ft_buf_set_data(e->fds[i].buf_write, e->fds[cs].name,
				ft_strlen(e->fds[cs].name), "ERROR: too much data"))
				return (EXIT_FAILURE);
			if (ft_buf_set_data(e->fds[i].buf_write, ": ",
			2, "ERROR: too much data"))
				return (EXIT_FAILURE);
			if (ft_buf_set_data(e->fds[i].buf_write, buf, ft_strlen(buf),
			"ERROR: too much data"))
				return (EXIT_FAILURE);
			if (ft_buf_set_data(e->fds[i].buf_write, "\n", 1,
			"ERROR: too much data"))
				return (EXIT_FAILURE);
			return (EXIT_SUCCESS);
		}
	}
	return (EXIT_FAILURE);
}

int			message(t_env *e, int cs, char *cmd)
{
	char	name_target[MAX_LEN_LOGIN_NAME];
	int		len;

	ft_bzero(name_target, MAX_LEN_LOGIN_NAME);
	len = get_name(cmd, name_target);
	if (len == -1)
	{
		buf_send_error(e->fds[cs].buf_write,
			NICKNAME_NEEDED, ft_strlen(NICKNAME_NEEDED));
		return (EXIT_FAILURE);
	}
	send_dada(e, cs, cmd + len, name_target);
	return (EXIT_SUCCESS);
}
