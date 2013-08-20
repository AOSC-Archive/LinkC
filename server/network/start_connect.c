/*##################################################
 * 开始连接
 *##################################################*/

#include "linkc_db.h"
#include "linkc_network.h"
#include "linkc_qmsg.h"
#include "p2p_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#define DEBUG		1		// DEBUG模
#define PORT		2341		// 端口


int start_connect(void)
{
	int i;
	int client_sockfd;
	struct user_data client;
	socklen_t len;
	int result = init_sqlite3_db();				// 初始化数据库
	if (result != 1)
	{
		printf ("DB Init error!\n");
		exit(EXIT_FAILURE);
	}
	int sockfd = init_network(PORT);				// 初始化网络模块
	if (sockfd == -1)
	{
		printf ("network_init error!\n");
		exit(EXIT_FAILURE);
	}
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
