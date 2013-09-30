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
#define IP_ADDR	"58.215.179.37"

void str_echo(int sockfd);

int main(int argc,char **argv)
{
	if(argv[1] == NULL)
	{
		printf("输入目标IP地址\n");
		return 0;
	}
	struct sockaddr_in Dest;
	Dest.sin_addr.s_addr=inet_addr(argv[1]);	// Save IP
	char buffer[MAXBUF];
	socklen_t addr_len = sizeof(struct sockaddr_in);
	int PrimaryUDP,PrimaryTCP,peer;
	struct sockaddr_in helper_addr,local_addr;
	int i; 		//tmp argument
	struct timeval tv;
	tv.tv_sec = 5;		// 5 second timed out
	tv.tv_usec = 0;
	int len = sizeof(tv);
       	
	if ((PrimaryUDP = socket(AF_INET,SOCK_DGRAM,0)) == -1)	// Creat UDP Socket
	{
		perror("Socket");
		exit(1);
	}
/*	if ((PrimaryTCP = socket(AF_INET,SOCK_STREAM,0)) == -1)	// Creat TCP Socket
	{
		perror("Socket");
		exit(1);
	}

	setsockopt(PrimaryUDP,SOL_SOCKET,SO_REUSEADDR,&opt,len);
	setsockopt(PrimaryTCP,SOL_SOCKET,SO_REUSEADDR,&opt,len);	// Set reuseable address*/

        local_addr.sin_family = AF_INET;
        local_addr.sin_port = 0;	//random Port
	local_addr.sin_addr.s_addr = INADDR_ANY;

	helper_addr.sin_family = AF_INET;
	helper_addr.sin_port = htons(2342);
	inet_pton(AF_INET,IP_ADDR,&helper_addr.sin_addr);

        if (bind(PrimaryUDP,(struct sockaddr *)&local_addr,sizeof(local_addr)) == -1)	// Bind Address for UDP Socket
        {
                perror("Bind");
                exit(1);
        }
/*        if (bind(PrimaryTCP,(struct sockaddr *)&local_addr,sizeof(local_addr)) == -1)	// Bind Address for TCP Socket
        {
                perror("Bind");
                exit(1);
        }*/
       
	printf("Connecting to server.....\n");
	if ((sendto(PrimaryUDP,(void *)&(Dest.sin_addr.s_addr),sizeof(ip_t),0,(struct sockaddr *)&helper_addr,addr_len)) < 0)
	{
		perror("FAILURE");
		close (PrimaryUDP);
//		close (PrimaryTCP);
		return 0;
	}
	
	printf("Done.\n");
	printf("Waiting for peer.........\n");
	if ((recvfrom(PrimaryUDP,buffer,sizeof(struct sockaddr_in),0,(struct sockaddr *)&helper_addr,&addr_len)) < 0)
	{
		perror("Recvfrom");
		close (PrimaryUDP);
//		close (PrimaryTCP);
		return 0;
	}

	printf("Done\n");
	memcpy((void *)&Dest,buffer,sizeof(struct sockaddr_in));
	printf("Connecting to peer.......\n");
	setsockopt(PrimaryUDP,SOL_SOCKET,SO_RCVTIMEO,(void *)&tv,len);
	setsockopt(PrimaryUDP,SOL_SOCKET,SO_SNDTIMEO,(void *)&tv,len);

	for (i=0;i<MAXTRY;i++)
	{
		strncpy(buffer,"1",1);
		buffer[1] = '\0';
		if(( sendto(PrimaryUDP,buffer,MAXBUF,0,(struct sockaddr *)&Dest,addr_len)) <= 0)
		{
			perror("FAILED");
			continue;
		}
		strncpy(buffer,"0",1);
		if((recvfrom(PrimaryUDP,buffer,MAXBUF,0,(struct sockaddr *)&Dest,&addr_len)) <= 0)
		{
			perror("FAILED");
			continue;
		}
		if(atoi(buffer)==1)
		{
			sendto(PrimaryUDP,buffer,MAXBUF,0,(struct sockaddr *)&Dest,addr_len);
			break;
		}
		sleep(1);
	}
	printf("Done.\n");
	testing_ui(PrimaryUDP,Dest);

	

	close (PrimaryUDP);
	close (PrimaryTCP);
	return 0;
}

