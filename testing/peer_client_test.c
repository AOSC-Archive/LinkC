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
		printf("Please Input Peer's IP!\n");
		return 0;
	}
	struct 	sockaddr_in Dest;
	struct 	timeval tv;					// for saving Times
	char 	buffer[MAXBUF];					// Buffer
	socklen_t addr_len	= sizeof(struct sockaddr_in);	// addr's Size
	int 	addr_size	= sizeof(struct sockaddr_in);	// addr's Size
	int 	len		= sizeof(tv);			// Time's Len
	int 	PrimaryUDP,PrimaryTCP;				// Socket
	struct 	sockaddr_in helper_addr,local_addr;		// Address
	int 	i; 						// tmp argument
	tv.tv_sec = 5;						// 5 second timed out
	tv.tv_usec = 0;						// 0 mic second
	Dest.sin_addr.s_addr=inet_addr(argv[1]);		// Save IP
       	
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
	setsockopt(PrimaryTCP,SOL_SOCKET,SO_REUSEADDR,&opt,len);// Set address reuseable*/

	local_addr.sin_family = AF_INET;			// UNIX SOCKET
	local_addr.sin_port = 0;				// Random Port[System will distribute the program a Random Port(Port>1024)]
	local_addr.sin_addr.s_addr = INADDR_ANY;		// Random IP[It's Meaning that Use All The IP Addr we have]

	helper_addr.sin_family = AF_INET;			// UNIX SOCKET
	helper_addr.sin_port = htons(2342);			// Set Helper's Port
	inet_pton(AF_INET,IP_ADDR,&helper_addr.sin_addr);	// Set Helper's IP address

	if (bind(PrimaryUDP,(struct sockaddr *)&local_addr,sizeof(local_addr)) == -1)	// Bind Address for UDP Socket
	{
		perror("Bind");
		exit(1);
        }
/*	if (bind(PrimaryTCP,(struct sockaddr *)&local_addr,sizeof(local_addr)) == -1)	// Bind Address for TCP Socket
	{
		perror("Bind");
		exit(1);
	}*/
       
	printf("Connecting to server.....\n");
	/* Tell Helper Who You Want Chat With */
	if ((sendto(PrimaryUDP,(void *)&(Dest.sin_addr.s_addr),sizeof(ip_t),MSG_DONTWAIT,(struct sockaddr *)&helper_addr,addr_len)) <= 0)
	{
		perror("Sendto");
		close (PrimaryUDP);
//		close (PrimaryTCP);
		return 0;
	}
	
	printf("Done.\n");
	printf("Waiting for peer.........\n");
	/* Waiting fot The Helper's Reply */
	if ((recvfrom(PrimaryUDP,buffer,sizeof(struct sockaddr_in),0,(struct sockaddr *)&helper_addr,&addr_len)) <= 0)
	{
		perror("Recvfrom");
		close (PrimaryUDP);
//		close (PrimaryTCP);
		return 0;
	}
	printf("Done\n");
	memcpy((void *)&Dest,buffer,sizeof(struct sockaddr_in));	// Save Dest's Address
	printf("Connecting to peer.......\n");
	setsockopt(PrimaryUDP,SOL_SOCKET,SO_RCVTIMEO,(void *)&tv,len);	// Set Recv Time Out
	setsockopt(PrimaryUDP,SOL_SOCKET,SO_SNDTIMEO,(void *)&tv,len);	// Set Send Time Out

	for (i=0;i<MAXTRY;i++)
	{
		strncpy(buffer,"1",1);
		buffer[1] = '\0';
		if((sendto(PrimaryUDP,buffer,MAXBUF,MSG_DONTWAIT,(struct sockaddr *)&Dest,addr_len)) <= 0)
		{
			perror("Send");
			continue;
		}
		strncpy(buffer,"0",1);
		if((recvfrom(PrimaryUDP,buffer,MAXBUF,0,(struct sockaddr *)&Dest,&addr_len)) <= 0)
		{
			perror("Recv");
			continue;
		}
		if(atoi(buffer)==1)
		{
			sendto(PrimaryUDP,buffer,MAXBUF,MSG_DONTWAIT,(struct sockaddr *)&Dest,addr_len);
			break;
		}
		sleep(1);
	}
	printf("Done.\n");
	testing_ui(PrimaryUDP,Dest);

	

	close (PrimaryUDP);
//	close (PrimaryTCP);
	return 0;
}

