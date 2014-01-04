/*##################################################
 * 开始连接
 *##################################################*/

#include "linkc_db.h"
#include "linkc_network.h"
#include "p2p_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#define DEBUG		1		// DEBUG模
#define PORT		2341		// 端口

int init_network(int port)
{
	int sockfd;					//网络句柄
	socklen_t len;					//长度
	struct sockaddr_in local_addr;	
	memset(&local_addr,0,sizeof(local_addr));
	if ((sockfd = socket (AF_INET,SOCK_STREAM, 0)) < 0)
	{
		perror ("Socket");
		exit (EXIT_FAILURE);
	}
	local_addr.sin_port = htons (port);
	local_addr.sin_family = AF_INET;

	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);


	len = sizeof(local_addr);

	bind (sockfd, (struct sockaddr *)&local_addr, len);
	listen (sockfd,5);
	return sockfd;
}

int start_connect(void)
{
	struct user_data client;
	socklen_t len;
	int result = init_sqlite3_db();				// 初始化数据库
	if (result != LINKC_SUCCESS)
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
