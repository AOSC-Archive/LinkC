/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： Aprli 2, 2014
 */

#include "def.h"
#include "linkc_network.h"
#include "linkc_network_protocol.h"
#include <string.h>

#define DEBUG	1
int send_friends_data(struct user_data user,void *data)
{
	char buffer[MAXBUF];
	int friend_count,length,byte,tmp,result;
	struct friend_data *My_friend;
	friend_count = get_friends_data (user.UID,&My_friend);
	((LSS*)data)->Action = USER_FRIENDS_DATA;
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
		((LMH*)buffer)->Totle = friend_count;			// 将好友数量放这里......我TM太聪明了
		result = send (user.sockfd,buffer,length,0);	// 发送 执行成功
#if DEBUG
		printf ("UID = %d\nHave %d friend(s)\n",user.UID,friend_count);
		printf ("------Friends------\n");
		for (tmp=0;tmp<friend_count;tmp++)	printf ("\tUID\t= %d\tNAME\t= %s\n",My_friend[tmp].info.UID,My_friend[tmp].info.username);
		printf ("------End----------\n");
#endif
		non_std_m_message_send(My_friend,user.sockfd,friend_count,sizeof(friend_data),USER_FRIENDS_DATA,0);
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
	send (user.sockfd,buffer,length,0);	// 执行情况
	non_std_m_message_send(MyFriend,user.sockfd,1,sizeof(friend_data),SYS_FRIEND_DATA,0);
	free (MyFriend);
	MyFriend = NULL;
	return 0;
}
