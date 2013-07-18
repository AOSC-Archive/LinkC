/*##################################################
 * 开始连接
 *##################################################*/

#include "../include/linkc_db.h"
#include "../include/linkc_network.h"
#include "../include/linkc_qmsg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#define DEBUG		1		// DEBUG模


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
	int sockfd = init_network();				// 初始化网络模块
	if (sockfd == -1)
	{
		printf ("network_init error!\n");
		exit(EXIT_FAILURE);
	}
#if QMSG
	key_t key;
	if ((key = ftok(".",'A')) == -1)
	{
		perror ("ftok");
		exit (EXIT_FAILURE);
	}
	if ((msgid = msgget (key,0666 | IPC_CREAT)) == -1)	// 创建消息队列
	{
		perror ("msgget");
		exit (EXIT_FAILURE);
	}
	if (result != 1)	exit (EXIT_FAILURE);

	pthread_t qmsg_id;
	pthread_create (&qmsg_id,NULL,(void *)*qmsg,&msgid);
#endif
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
//		pid=fork();
//		if (pid == 0)	keep_connect(&client);
	}
}

