#ifndef _LINKC_NETWORK_H_
#define _LINKC_NETWORK_H_

#include <sys/types.h>
#include "linkc_types.h"

extern int start_connect (void);
/* 初始化连接，并等待连接 */

extern int init_network(int port);
/* 初始化网络部分 */

extern int keep_connect (struct user_data* _user);
/*
 * 建立连接后，开始接受客户端数据
 * 参数：1	struct user_data 的地址
 */

extern int send_friends_data	(struct user_data user,void *data);
extern int send_friend_data	(struct user_data user,void *data);

#endif
