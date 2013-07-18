#ifndef _LINKC_NETWORK_H_
#define _LINKC_NETWORK_H_

#include <sys/types.h>
#include "../include/server_data_type.h"
//#include "user_login.c"

extern int start_connect (void);
/* 初始化连接，并等待连接 */

extern int init_network(void);
/* 初始化网络部分 */

extern int keep_connect (struct user_data* _user);
/* 
 * 建立连接后，开始接受客户端数据 
 * 参数：1	struct user_data 的地址
 */

#endif
#ifndef _LINKC_NETWORK_

#define IP	"127.0.0.1"	// IP地址
#define PORT	2341		// 服务端口

#endif
