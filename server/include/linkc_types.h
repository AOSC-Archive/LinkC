#ifndef LINKC_TYPES_H
#define LINKC_TYPES_H
#define MAXBUF		1024

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

#ifndef _LINKC_

#define LINKC_OK		"OK"
#define LINKC_FAILURE		"Failure"
#define LINKC_ERROR		"Error"
#define LINKC_FULL		"Full"
#define LINKC_TRY_SO_MANY	"Tried_So_Many"
#define LINKC_TIMED_OUT		"Timed_Out"
#define LINKC_LOGIN		"Action_Login"
#define LINKC_LOGOUT		"Action_Logout"
#define LINKC_GET_FRIENDS	"Get_Friends_Data"	//这个是获得所有好友数据
#define LINKC_GET_FRIEND	"Get_Friend_Data"	//这是对指定好友数据获取
#define LINKC_NO_FRIEND		"No_Friend"
#define LINKC_CHAT_WANT		"Chat_Want" 
#define LINKC_MESSAGE		"YOUR_MESSAGE"
#define LINKC_QUIT		"QUIT"

#endif


