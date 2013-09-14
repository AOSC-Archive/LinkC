#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "def.h"
#include "conn_list.h"
#define MAXBUF	512
#define MAXTRY	10

void str_echo(int sockfd);

int main(int argc,char **argv)
{
	if(argv[1] == NULL)
	{
		printf("输入目标IP地址\n");
		return 0;
	}
	if(argv[2] == NULL)
	{
		printf("您的终端是否为服务的端[1]是 [0]不是\n");
		return 0;
	}
	int is_server = atoi(argv[2]);
	conn_list_item item;
	item.info.Dest.sin_addr.s_addr=inet_addr(argv[1]);	// Save IP
	printf("%s\n",argv[1]);			// Check IP
	char buffer[MAXBUF];
	socklen_t addr_len = sizeof(struct sockaddr_in);
	int PrimaryUDP,PrimaryTCP,peer;
	struct sockaddr_in helper_addr,local_addr,peer_addr;
	int opt = 1;
	int len = sizeof(opt);
       	
	if ((PrimaryUDP = socket(AF_INET,SOCK_DGRAM,0)) == -1)	// Creat UDP Socket
	{
		perror("Socket");
		exit(1);
	}
	if ((PrimaryTCP = socket(AF_INET,SOCK_STREAM,0)) == -1)	// Creat TCP Socket
	{
		perror("Socket");
		exit(1);
	}

	setsockopt(PrimaryUDP,SOL_SOCKET,SO_REUSEADDR,&opt,len);
	setsockopt(PrimaryTCP,SOL_SOCKET,SO_REUSEADDR,&opt,len);	// Set reuseable address

        local_addr.sin_family = AF_INET;
        local_addr.sin_port = 0;	//random Port
	local_addr.sin_addr.s_addr = INADDR_ANY;

	helper_addr.sin_family = AF_INET;
	helper_addr.sin_port = htons(2342);
	inet_pton(AF_INET,"117.59.14.114",&helper_addr.sin_addr);

        if (bind(PrimaryUDP,(struct sockaddr *)&local_addr,sizeof(local_addr)) == -1)	// Bind Address for UDP Socket
        {
                perror("Bind");
                exit(1);
        }
        if (bind(PrimaryTCP,(struct sockaddr *)&local_addr,sizeof(local_addr)) == -1)	// Bind Address for TCP Socket
        {
                perror("Bind");
                exit(1);
        }
       
        printf("Server IP\t=%s\t\tPort\t=%d\n",inet_ntoa(helper_addr.sin_addr),ntohs(helper_addr.sin_port));
	
	getchar();

	memcpy(buffer,(void *)&item,sizeof(conn_list_item));
	if ((sendto(PrimaryUDP,buffer,MAXBUF,0,(struct sockaddr *)&helper_addr,addr_len)) < 0)
	{
		perror("sendto");
		close (PrimaryUDP);
		close (PrimaryUDP);
		return 0;
	}
	
	if ((recvfrom(PrimaryUDP,buffer,MAXBUF,0,(struct sockaddr *)&helper_addr,&addr_len)) < 0)
	{
		perror("sendto");
		close (PrimaryUDP);
		close (PrimaryUDP);
		return 0;
	}
	conn_info info;
	memcpy((void *)&info,buffer,sizeof(conn_info));
	printf("DestIP\t=%s\n",inet_ntoa(info.Dest.sin_addr));

	int i;
	for (i=0;i<MAXTRY;i++)
	{
		strncpy(buffer,"1",1);
		buffer[1] = '\0';
		sendto(PrimaryUDP,buffer,MAXBUF,0,(struct sockaddr *)&(info.Dest),addr_len);
		strncpy(buffer,"0",1);
		recvfrom(PrimaryUDP,buffer,MAXBUF,0,(struct sockaddr *)&(info.Dest),&addr_len);
		printf("buffer =%s\n",buffer);
		if(atoi(buffer)==1)	break;
	}
	len = sizeof(info.Dest);
	if (is_server == 1)		// 确定是服务端
	{
		if (listen(PrimaryTCP,2) == -1)	// 添加监听
		{
			perror("listen");	
			return 0;
		}
		if(peer = accept(PrimaryTCP,(struct sockaddr *)&(info.Dest),&addr_len)==-1)
		{
			perror("accept");
			return 0;
		}
		printf("Connect successful!\n");
	}
	else
	{
		sleep(1); //sleep a while so the dest can perpare for this connection
		if (connect(PrimaryTCP,(struct sockaddr *)&(info.Dest),sizeof(info.Dest)) == -1)
		{
			perror("connect");
			return 0;
		}
		
	}

	close (PrimaryUDP);
	close (PrimaryUDP);
	return 0;
}
