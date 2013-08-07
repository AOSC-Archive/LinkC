/*##################################################
 * 开始连接
 *##################################################*/

#include "linkc_db.h"
#include "linkc_network.h"
#include "linkc_qmsg.h"
#include "def.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#define DEBUG		1		// DEBUG模


int start_connect(void)
{
	int i;
	int client_sockfd,sockfd;
	struct user_data client;
	socklen_t len;
	CHECK_FAILED(init_sqlite3_db(),ERROR_DB_INIT);				// 初始化数据库
	CHECK_FAILED(sockfd = init_network(),ERROR_NETWORK_INIT);				// 初始化网络模块

	len = sizeof (client.addr);
	pthread_t pid;
	while (1)
	{
#if DEBUG
		sleep (1);
		printf ("Server is waiting for connect!\n");
#endif
		client.sockfd = accept (sockfd,(struct sockaddr *)&client.addr,(socklen_t *)&len);

		pthread_create (&pid,NULL,(void*)*keep_connect,&(client));
	}
}

