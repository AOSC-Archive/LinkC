#ifndef LINKC_TYPES_H
#define LINKC_TYPES_H
#define MAXBUF		512

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include "def.h"

#define uint32_t	id_t

typedef struct login_data
{
        char user_name[12];
        char pass_word[20];
}login_data;

typedef struct user_data
{
	id_t UID;
	int sockfd;
	struct sockaddr_in addr;
	struct login_data login;
}user_data;

typedef struct friend_data
{
	id_t UID;
	char name[20];
	char nickname[20];
	int group;
	char telephone[20];
	char company[50];
	char address[80];
	char join_time[25];
	char last_login[25];
	ip_t ip;
	int status;
}friend_data;


#endif



