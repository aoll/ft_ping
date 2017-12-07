/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aollivie <aollivie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/17 13:34:01 by aollivie          #+#    #+#             */
/*   Updated: 2017/12/01 03:19:26 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

#include <sys/types.h>
#include <sys/wait.h>
#include "ft_p.h"
#include "ft_irc.h"

# define GUEST					"guest"
# define NB_MAX_CHANNEL			42
# define MAX_LEN_CHANEL_NAME	9
# define MAX_LEN_LOGIN_NAME 	9

# define JOIN_CHANNEL			"/join"
# define NEW_CHANNEL			"/newchannel"
# define LEAVE_CHANNEL			"/leave"
# define NICKNAME				"/nick"
# define WHO					"/who"
# define MSG					"/msg"

# define INTERN_ERROR			"ERROR: intern error, please retry\n"
# define WHO_NEED_CHANNEL		"ERROR: /who: must join a channel\n"
# define WHO_NEED_NO_ARGS		"ERROR: /who need no argument\n"
# define BUFFER_FULL			"ERROR: buffer full\n"
# define LEAVE_CHANNEL_NO_ARGS	"ERROR: /leave need no argument\n"
# define NEWCHANNEL_NEED_NAME	"ERROR: /newchannel need <arg>\n"
# define CHANNEL_NO_EXIST		"ERROR: channel not exist\n"
# define CHANNEL_EXIST			"ERROR: channel already exist\n"
# define NICKNAME_TO_LONG		"ERROR: nickname to long\n"
# define NICKNAME_NOT_SPACE		"ERROR: space not allowed in a nickname\n"
# define NICKNAME_NEEDED		"ERROR: need nickname\n"

typedef struct	s_buf
{
	size_t		size;
	char		*start_buf;
	char		*end_buf;
	char		*start_data;
	char		*end_data;
}	t_buf;

typedef struct	s_fd
{
	int			type;
	void		(*fct_read)();
	void		(*fct_write)();
	char		name[MAX_LEN_LOGIN_NAME];
	t_buf		*buf_write;
	t_buf		*buf_read;
	char		buf_read1[BUF_SIZE + 1];
	char		buf_write1[BUF_SIZE + 1];
	char		channel[MAX_LEN_CHANEL_NAME];
}	t_fd;

typedef struct	s_chanel
{
	char			*name;
	int				*fds;
	int				fdsize;
}	t_chanel;

typedef struct	s_chanels
{
	int			nb_chanel;
	int			nb_chanel_active;
	t_chanel	*l_chanel;
}	t_chanels;

typedef struct	s_env
{
	t_fd		*fds;
	t_chanels	*channels;
	int			port;
	int			maxfd;
	int			max;
	int			r;
	fd_set		fd_read;
	fd_set		fd_write;

}	t_env;


int			buf_send_error(t_buf *buf, const char *err_mess, size_t size);
//cmd
int			set_name(t_env *e, int cs, char *cmd);
int			who(t_env *e, int cs, char *cmd);
int			join_channel(t_env *e, int cs, char *cmd);
int			leave_channel(t_env *e, int cs, char *cmd);
int			new_channel(t_env *e, int cs, char *cmd);
int			message(t_env *e, int cs, char *cmd);

//ft_irc
int			start_server(int port);
int			create_server(int port, int nb_conn);

//env
int			init_env(t_env *e, int port);
void		clean_fd(t_fd *fd);
void		client_read(t_env *e, int cs);
void		client_write(t_env *e, int cs);

//buffer_circulaire
t_buf		*ft_buf_new(size_t size);
int			ft_buf_get_data(t_buf *buf_e, char *dest);
int			ft_buf_add_data(t_buf *buf_e, const char *s, size_t size);
int	ft_buf_set_data(t_buf *buf, const char *data, size_t size, char *err_mess);
void		ft_buf_clean(t_buf *buf_e);

//chanel
t_chanels	*ft_chanels_init(size_t nb_chanel);
int			ft_new_chanel(t_chanels *cha, const char *name, size_t nb_fd);
int			ft_join_chanel(t_chanels *cha, const char *name, int fd);
int			ft_leave_chanel(t_chanels *cha, const char *name, int fd);
int			ft_is_channel_exist(t_chanels *cha, const char *name);

//ft_p
int			fork_process_cmd(int fd, char **arg);
int			cd_requet(t_cs *cs, char **requet);
int			verify_dest(t_cs *cs, char *dest);
int			verify_multi_dest(t_cs *cs, char **requet);
int			new_process(int fd);
int			free_cs(t_cs *cs);
int			init_cs(t_cs *cs, int fd);
int			cmd_requet(t_cs *cs, char **requet);
int			quit_requet(t_cs *cs);
int			pwd_requet(t_cs *cs, char **requet);
int			put_requet(t_cs *cs, char **requet, char *requet_s);
int			get_requet_server(t_cs *cs, char **requet);

#endif
