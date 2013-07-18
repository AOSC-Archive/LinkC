#include "../include/linkc_network.h"
#include "../include/linkc_qmsg.h"
#include <string.h>
#include <stdlib.h>
#define _LINkC_NETWORK_	// 定义IP地址和PORT

int init_network(void)
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
	local_addr.sin_port = htons (PORT);
	local_addr.sin_family = AF_INET;

	local_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	len = sizeof(local_addr);

	bind (sockfd, (struct sockaddr *)&local_addr, len);
	listen (sockfd,5);
	printf ("Sockfd = %d\n",sockfd);
	return sockfd;
}
