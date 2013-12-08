#ifndef LINKC_TYPES_H
#define LINKC_TYPES_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

typedef int errorcode;
typedef int flag_t;
typedef unsigned long ip_t;
typedef unsigned short port_t;

struct message_t
{
        uint8_t header;
        time_t check_time;
        char message[512];
};

typedef struct login_data
{
        char user_name[12];
        char pass_word[20];
}login_data;

struct conn_info_t
{
        struct sockaddr_in Src;
        struct sockaddr_in Dest;
};
typedef struct conn_info_t conn_info;

typedef struct friend_data
{
	id_t UID;
	char name [20];
	char nickname [20];		//可以为空，空为“NULL”
	int group;
//-------来自客户端的User_data---------//
	char telephone [20];
	char company [50];
	char address [80];
	char join_time [25];
	char last_login [25];
	ip_t ip;
	int state;

}friend_data;


#endif

