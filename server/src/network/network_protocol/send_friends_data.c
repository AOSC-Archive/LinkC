#include "def.h"
#include "linkc_network.h"
#include "linkc_network_protocol.h"
#include <string.h>

int send_friends_data(struct user_data user,void *data)
{
	char buffer[MAXBUF];
	int friend_count,length,byte,tmp,result;
	struct friend_data *My_friend;
	friend_count = get_friends_data (user.UID,&My_friend);
	if (friend_count == 0)		// 如果好友个数为 0
	{
		((LinkC_Sys_Status *)data)->Status = LINKC_FAILURE;
		length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer);
		result = TCP_Send (user.sockfd,buffer,length,0);	// 发送 没有好友
		My_friend = NULL;
	}
	else if (friend_count < 0)	// 如果执行失败
	{
		((LinkC_Sys_Status *)data)->Status = LINKC_FAILURE;
		length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer);
		result = TCP_Send (user.sockfd,buffer,length,0);		// 发送 错误
		My_friend = NULL;
		if (byte < 0)
			return -1;
	}
	else
	{
		((LinkC_Sys_Status *)data)->Status = SUCCESS;
		length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer);
		result = TCP_Send (user.sockfd,buffer,length,0);	// 发送 执行成功
#if DEBUG
		printf ("UID = %d\nHave %d friend(s)\n",user.UID,friend_count);
		printf ("------Friends------\n");
		for (tmp=0;tmp<friend_count;tmp++)	printf ("\tUID\t= %d\tNAME\t= %s\n",My_friend[tmp].UID,My_friend[tmp].name);
		printf ("------End----------\n");
#endif
		non_std_m_message_send(My_friend,user.sockfd,friend_count,sizeof(friend_data),SYS_FRIEND_DATA,0);
		free(My_friend);
		My_friend = NULL;
	}
	return 0;
}

int send_friend_data(struct user_data user,void *data)
{
	struct friend_data *MyFriend;
	int length;
	int tmp;
	char buffer[256];
	tmp = get_friend_data (user.UID,((LUR*)data)->UID,&MyFriend);
	((LSS*)data)->Action = USER_FRIEND_DATA;
	if(tmp != LINKC_SUCCESS)
	{
		((LSS*)data)->Status = LINKC_FAILURE;
		length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer);
		TCP_Send(user.sockfd,buffer,length,0);
	}
	((LSS*)data)->Status = LINKC_SUCCESS;
	length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer);
	TCP_Send (user.sockfd,buffer,length,0);	// 执行情况
	printf("Sended!\n");
	TCP_Send (user.sockfd,MyFriend,sizeof(friend_data),0);
	free (MyFriend);
	return 0;
}
