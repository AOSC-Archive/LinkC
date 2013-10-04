#include "conn_list.h"
#include "p2p_helper.h"
#include "def.h"
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
extern int init_network(int port);

int ip_size 	= sizeof(ip_t);
int addr_size	= sizeof(struct sockaddr_in);
socklen_t len	= sizeof(struct sockaddr_in);

int p2p_helper(void)
{
	int sockfd;		// Sockfd
	int i;			// Tmp
	conn_list list;		// Connection List
	conn_list_item item;	// item
	socklen_t len;		// Address's Lenth
	struct sockaddr_in addr;// save addr
	if ((sockfd = Network_init(2342)) < 0)			// Init Network
		return ERROR_NETWORK_INIT;			// Return Error
	CHECK_FAILED(conn_list_init(&(list)),ERROR_LIST_INIT);	// Init List


	while(1)
	{
		recvfrom(sockfd,(void *)&(item.info.Dest.sin_addr.s_addr),ip_size,0,(struct sockaddr *)&addr,&len);	// Recv Dest IP
		item.info.Src=addr;									// Save Src Addr
		printf("DestIP=%s\n",inet_ntoa(item.info.Dest.sin_addr));				// Debug
		printf("SrcIP\t=%s\nSrcPort\t=%d\n",inet_ntoa(item.info.Src.sin_addr),addr.sin_port);	// Debug
		i = conn_list_find(&list,item.info,&(item.info.Dest));					// Search for This IP
		if (i == -14)										// If Not Found
		{
			conn_list_add(&list,item);							// Add item
			continue;
		}
		sendto(sockfd,(void *)&(item.info.Dest),addr_size,MSG_DONTWAIT,(struct sockaddr *)&(item.info.Src),len);
		sendto(sockfd,(void *)&(item.info.Src),addr_size,MSG_DONTWAIT,(struct sockaddr *)&(item.info.Dest),len);
		conn_list_remove(&list,item.info);							// remove item
	}
	exit(0);
}


int Network_init(int port)
{
	int sockfd;					// Network Socket
	struct sockaddr_in local_addr;
	memset(&local_addr,0,addr_size);
	if ((sockfd = socket (AF_INET,SOCK_DGRAM, 0)) < 0)	// UDP Socket
	{
		perror ("Socket");
		exit (EXIT_FAILURE);
	}
	local_addr.sin_port = htons (port);
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if((bind (sockfd, (struct sockaddr *)&local_addr, len)) == -1)
	{
		perror("bind");
		return (-1);
	}
	return (sockfd);
}
