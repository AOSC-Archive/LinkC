#define PORT	2342	// 定义端口

#include "p2p_helper.h"
#include "linkc_network.h"
#include "def.h"
#include "conn_list.h"
#include <stdio.h>
#include <string.h>

errorcode p2p_helper_start(void)
{
	int sockfd = init_network(PORT);
	if (sockfd < 0)
		return ERROR_NETWORK_INIT;
	peer_data client;
	CHECK_FAILED(conn_list_init(&(client.list)),ERROR_LIST_INIT);	// 初始化池

	socklen_t len;		// 长度
	pthread_t pid;		// 线程ID
	while (1)
	{
#if DEBUG
		sleep (1);
		printf ("Server is waiting for connect!\n");
#endif
		client.sockfd = accept (sockfd,(struct sockaddr *)&client.addr,(socklen_t *)&len);

		pthread_create (&pid,NULL,(void*)*p2p_helper,&(client));
        }
	return SUCCESS;
}


errorcode p2p_helper(peer_data *data)
{
	conn_list_item item;		// item
	socklen_t size;			// size
	char buffer[512];		// buffer
	struct timeval tv;		// timeval
	struct sockaddr_in addr = data->addr;	// save addr
	int sockfd = data->sockfd;		// save sockfd

	CHECK_FAILED(recv(sockfd,buffer,512,MSG_WAITALL),ERROR_RECV_MSG);
	memcpy((void *)&item,buffer,sizeof(conn_list_item));

	
	conn_list_item *find;
	if (conn_list_find(&(data->list),item.info.DestIP,&find) == -14);	//If Not Found
	{
		item.info.SrcIP = addr.sin_addr.s_addr;
		item.info.SrcPort = addr.sin_port;
		conn_list_add(&(data->list),item);		// Add item
		sleep(25);					// Sleep 25 seconds
		conn_list_remove(&(data->list),item.info.DestIP);	// remove item
		close(sockfd);		// close connection
		return SUCCESS;		// return
	}
	find->info.DestPort = addr.sin_port;

	memcoy((void *)&buffer,find,sizeof(conn_list_item));
	send(sockfd,buffer,512,MSG_WAITALL);
	sleep(10);
	close(sockfd);
	return SUCCESS;
}
