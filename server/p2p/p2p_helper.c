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
	int sockfd = Network_init(2342);
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
		memcpy ((void*)&item,buffer,sizeof(conn_list_item));
		printf("DestIP=%s\n",inet_ntoa(item.info.Dest.sin_addr));
		item.info.Src=addr;

		int i = conn_list_find(&list,item.info,&(item.info.Dest));
		printf("SrcIP\t=%s\nSrcPort\t=%d\n",inet_ntoa(item.info.Src.sin_addr.s_addr),addr.sin_port);
		if (i == -14)
		{
			conn_list_add(&list,item);		// Add item
//			conn_list_remove(&(data->list),item.info);	// remove item
//			return SUCCESS;		// return
			continue;
		}
		item.info.Src = addr;
		sendto(sockfd,buffer,512,0,(struct sockaddr *)&addr,len);
		tmp.Src=item.info.Dest;
		tmp.Dest=item.info.Src;
		addr = item.info.Dest;
		addr.sin_family = AF_INET;
		memcpy (buffer,(void *)&tmp,sizeof(conn_info));
		sendto(sockfd,buffer,512,0,(struct sockaddr *)&addr,len);
		conn_list_remove(&list,item.info);	// remove item
	}
	exit(0);
}


int Network_init(int port)
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
