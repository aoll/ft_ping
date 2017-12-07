/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 00:23:49 by alex              #+#    #+#             */
/*   Updated: 2017/12/01 02:40:58 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static int	ret_err(t_buf *buf)
{
	buf_send_error(buf,
		INTERN_ERROR, ft_strlen(INTERN_ERROR));
	return (EXIT_FAILLURE);
}

static int	who_add_list(t_env *e, int cs, int index_channel)
{
	int		i;
	int tmp;

	i = 0;
	while (i < e->maxfd)
	{
		tmp = e->channels->l_chanel[index_channel].fds[i];
		if (tmp == 1)
		{
			if (ft_buf_set_data(e->fds[cs].buf_write, e->fds[i].name,
				ft_strlen(e->fds[i].name), BUFFER_FULL))
				return (ret_err(e->fds[cs].buf_write));
			if (ft_buf_set_data(
				e->fds[cs].buf_write, " ", 1, BUFFER_FULL))
				return (ret_err(e->fds[cs].buf_write));
		}
		i++;
	}
	if (ft_buf_set_data(e->fds[cs].buf_write, "\n", 1, BUFFER_FULL))
		return (ret_err(e->fds[cs].buf_write));
	return (EXIT_SUCCESS);
}

int			who(t_env *e, int cs, char *cmd)
{
	int		index_channel;

	if (ft_strcmp(cmd, WHO))
	{
		return (buf_send_error(e->fds[cs].buf_write,
			WHO_NEED_NO_ARGS, ft_strlen(WHO_NEED_NO_ARGS)));
	}
	index_channel = ft_is_channel_exist(e->channels, e->fds[cs].channel);
	if (index_channel < 0)
	{
		printf("%s\n", WHO_NEED_CHANNEL);
		return (buf_send_error(e->fds[cs].buf_write,
			WHO_NEED_CHANNEL, ft_strlen(WHO_NEED_CHANNEL)));
	}
	return (who_add_list(e, cs, index_channel));
}
