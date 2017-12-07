/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_channel.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 00:58:46 by alex              #+#    #+#             */
/*   Updated: 2017/12/01 02:37:05 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static int	ret_err(const char *err_mess)
{
	printf("%s\n", err_mess);
	return (EXIT_FAILLURE);
}

/*
** create new channel
*/

int			new_channel(t_env *e, int cs, char *cmd)
{
	int		ret;
	char	*trim;

	if (!ft_strlen(cmd + ft_strlen(NEW_CHANNEL)))
	{
		buf_send_error(e->fds[cs].buf_write,
			NEWCHANNEL_NEED_NAME, ft_strlen(NEWCHANNEL_NEED_NAME));
		return (EXIT_FAILURE);
	}
	if (!(trim = ft_strtrim(cmd + ft_strlen(NEW_CHANNEL))))
	{
		buf_send_error(e->fds[cs].buf_write,
			INTERN_ERROR, ft_strlen(INTERN_ERROR));
		return (ret_err("ERROR: new_channel: intern error"));
	}
	if ((ret = ft_new_chanel(e->channels, trim, e->maxfd)))
		buf_send_error(e->fds[cs].buf_write,
			ret == -1 ? CHANNEL_EXIST : INTERN_ERROR,
			ft_strlen(ret == -1 ? CHANNEL_EXIST : INTERN_ERROR));
	free(trim);
	return (ret == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
