#include "conn_list.h"
#include "p2p_helper.h"
#include "def.h"
#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
extern int init_network(int port);

int p2p_helper(void)
{
	int sockfd = init_network(2342);
	if (sockfd < 0)
		return ERROR_NETWORK_INIT;
	conn_list list;
	CHECK_FAILED(conn_list_init(&(list)),ERROR_LIST_INIT);	// 初始化池

	socklen_t len;		// 长度

	conn_list_item item;		// item
	conn_info tmp;
	socklen_t size;			// size
	char buffer[512];		// buffer
	struct sockaddr_in addr;	// save addr
	len = sizeof (struct sockaddr_in);


	while(1)
	{
		recvfrom(sockfd,buffer,512,0,(struct sockaddr *)&addr,&len);
		memcpy((void *)&item,buffer,sizeof(conn_list_item));
		printf("DestIP\t=%s\n",inet_ntoa(item.info.DestIP));

		item.info.SrcIP = addr.sin_addr.s_addr;
		item.info.SrcPort = addr.sin_port;


		int i = conn_list_find(&list,item.info,&(item.info.DestPort));
		printf("SrcIP\t=%s\nDestIP\t=%s\n",inet_ntoa(item.info.SrcIP),inet_ntoa(item.info.DestIP));
		if (i == -14)
		{
			conn_list_add(&list,item);		// Add item
//			conn_list_remove(&(data->list),item.info);	// remove item
//			return SUCCESS;		// return
			continue;
		}
		item.info.SrcPort = addr.sin_port;
		memcpy (buffer,(void *)&(item.info),sizeof(conn_info));
		printf("SrcIP\t=%s\nDestIP\t=%s\nSrcPort\t=%d\nDestIP\t=%d\n",inet_ntoa(item.info.SrcIP),inet_ntoa(item.info.DestIP),item.info.SrcPort,item.info.DestPort);
		sendto(sockfd,buffer,512,0,(struct sockaddr *)&addr,len);
		tmp.SrcIP=item.info.DestIP;
		tmp.DestIP=item.info.SrcIP;
		tmp.SrcPort=item.info.DestPort;
		tmp.DestPort=item.info.SrcPort;
		addr.sin_addr.s_addr = item.info.DestIP;
		addr.sin_port = item.info.DestPort;
		addr.sin_family = AF_INET;
		memcpy (buffer,(void *)&tmp,sizeof(conn_info));
		printf("Sending...\n");
		if (sendto(sockfd,buffer,512,0,(struct sockaddr *)&addr,len) == -1)
		{
			perror("sendto");
			return 0;
		}
		printf("Send done\n");
		conn_list_remove(&list,item.info);	// remove item
	}
	exit(0);
}


int init_network(int port)
{
	int sockfd;					//网络句柄
	socklen_t len;					//长度
	struct sockaddr_in local_addr;	
	memset(&local_addr,0,sizeof(local_addr));
	if ((sockfd = socket (AF_INET,SOCK_DGRAM, 0)) < 0)
	{
		perror ("Socket");
		exit (EXIT_FAILURE);
	}
	local_addr.sin_port = htons (port);
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	len = sizeof(local_addr);

	if((bind (sockfd, (struct sockaddr *)&local_addr, len)) == -1)
	{
		perror("bind");
		return (-1);
	}
	return (sockfd);
}
