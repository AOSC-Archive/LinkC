/* ###################
 * 连接模块[仍在开发中]
   #################### */

#include "linkc_types.h"
#include "linkc_db.h"
#include "linkc_user.h"
#include "linkc_network_protocol.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#define DEBUG		1		// 是否为DEBUG模式
#define MAX_ERROR	5		// 最大错误次数
#define MAX_FAILURE_COUNT 5

#define LOGINCOUNT	2		// 最大登录尝试次数
extern void cls_buf(char * buffer,int size);

int msgid;
int keep_connect (struct user_data* _user)
{
	int i,friend_count,result,tmp,error_count,count;
	struct user_data user;				// 用户基本数据
	int failure_count = 0;				// 登录已经失败次数
	int byte;					// 接受的数据。
	socklen_t len = sizeof (_user -> addr);

	int sockfd = _user -> sockfd;			// 保存sockfd
	user.sockfd = _user -> sockfd;
	user.addr = _user -> addr;			// 保存addr
	struct friend_data* My_friend = NULL;		// 暂定

	int DEST_UID;

	char  buffer[MAXBUF + 1];			//缓存
	void *data = malloc(MAXBUF);
	uint16_t flag;
	uint16_t offset=0;
	uint16_t lenth;


	byte = recv(user.sockfd,buffer,MAXBUF,MSG_WAITALL);
	((LinkC_Sys_Status *)data)->Action = CONNECTION;
	if(check_message(buffer,byte) == CONNECTION)
		((LinkC_Sys_Status *)data)->Status = SUCCESS;
	else
		((LinkC_Sys_Status *)data)->Status = FAILURE;
	pack_message(SYS_ACTION_STATUS,data,SYS_STATUS_LENTH,buffer);
	byte = send (sockfd,LINKC_OK,MAXBUF,MSG_WAITALL);	//发送连接成功
#if DEBUG
	printf ("Socket\t= %d\n",sockfd);
	printf ("Connected!\n\tIP\t= %s\n\tPort\t= %d\n",inet_ntoa(user.addr.sin_addr),user.addr.sin_port);	/* 输出连接信息 */
#endif
start:
	byte = recv (sockfd,buffer+offset,MAXBUF,MSG_WAITALL);
	if (byte <= 0)		goto end;
	flag = check_message(buffer,byte+offset);
	if(flag == NOT_MESSAGE || flag == EXIT)	goto end;	/* 不是消息或者是退出请求 */
	if(flag == MESSAGE_INCOMPLETE){				/* 数据不完整 */
		offset += byte;					/* 设置偏移量 */
		if(offset > 1024)	goto end;
		goto start;
	}
	else	offset = 0;
	if (flag == USER_LOGIN)			/* 如果是登录请求 */
	{
	unpack_message(buffer,byte+offset,data);
	memcpy((void *)&(user.login),data,sizeof(login_data));
	result = user_login (&user);	/* 进行登录 , 获得username,password和UID*/
	((LinkC_Sys_Status *)data)->Action = USER_LOGIN;
	if (result < 0)
	{
		((LinkC_Sys_Status *)data)->Status = SYS_LOGIN_FAILURE;
		lenth = pack_message(SYS_ACTION_STATUS,data,SYS_STATUS_LENTH,buffer);
		send(user.sockfd,data,lenth,MSG_DONTWAIT);
		goto end;			/* 如果出现错误，则退出*/
	}
	else if(result == 0)			/* 如果登陆失败，则增加错误计数，然后continue */
	{
		count++;
		printf ("Login failure!\n");
		if (failure_count > MAX_FAILURE_COUNT)
			((LinkC_Sys_Status *)data)->Status = USER_LOGIN_LIMITED;
		else
			((LinkC_Sys_Status *)data)->Status = USER_LOGIN_FAILURE;
		lenth = pack_message(SYS_ACTION_STATUS,data,SYS_STATUS_LENTH,buffer);
		send(user.sockfd,data,lenth,MSG_DONTWAIT);
		if (count > LOGINCOUNT)
		{
#if DEBUG
			printf ("You tried so many times!\n");
#endif
			goto end;
		}
		goto start;
	}
	((LinkC_Sys_Status *)data)->Status = USER_LOGIN_SUCCESS;
	lenth = pack_message(SYS_ACTION_STATUS,data,SYS_STATUS_LENTH,buffer);
	byte = send (user.sockfd,data,lenth,MSG_WAITALL);
	error_count = 0;	/* 初始化错误个数 */
	offset = 0;		/* 初始化偏移量   */
	while (1)
	{
		if (error_count > MAX_ERROR)		// 如果超过最大错误允许范围
		{
#if DEBUG
			printf ("UID = %d cause so much errors!\n",user.UID);
#endif
			user_logout(user);
				goto end;
		}
		byte = recv (user.sockfd,buffer+offset,MAXBUF,MSG_WAITALL);
		if (byte <= 0)				// 如果接受错误，则增加一个错误计数
		{
			printf ("Recv Nothing!\n");
			error_count ++;
			continue;
		}
#if DEBUG
		sleep(1);
#endif
		flag = check_message(buffer,byte+offset);
		unpack_message(buffer,byte+offset,data);
		if(flag == MESSAGE_INCOMPLETE)
		{
			offset += byte;		// 设置偏移量
			if(offset > 1024)	goto end;
			continue;
		}
		byte += offset;
		offset = 0;
		if(flag == HEART_BEATS)	continue;
		if(flag == USER_REQUEST)
		{
			if(((LinkC_User_Request *)data)->Action == LOGOUT)	// 注销
			{
				user_logout(user);
				goto end;
			}
			else if(((LinkC_User_Request *)data)->Action == USER_FRIEND_DATA)	// 好友数据
			{
				if(((LinkC_User_Request *)data)->Flag == ALL_FRIEND)		// 若是获得全部好友数据
					send_friends_data(user);
				else
				{
					result = ((LinkC_User_Request *)data)->UID;
					printf("Get UID\t= %d\n",result);
					tmp = get_friend_data (user.UID,result,&My_friend);
					printf("Get Status\t= %d\n",tmp);
					if()
					memcpy(buffer,My_friend,sizeof(friend_data));
					lenth = pack_message(SYS_FRIEND_DATA,buffer,sizeof(friend_data),data);
					byte = send (sockfd,data,lenth,MSG_WAITALL);	// 发送好友信息
					free (My_friend);
				}
			}
		}
/*		switch(flag)
		{
			case MESSAGE_INCOMPLETE:	// 数据不完整
			{
				offset += byte;		// 设置偏移量
				if(offset > 1024)	goto end;
				continue;
			}
			offset = 0;
			case USER_LOGOUT:	// 若是注销
			{
				user_logout(user);
				goto end;
			}
			case USER_FRIEND_DATA:	//如果接受数据为 请求单个好友数据
			{
				byte = recv(user.sockfd,buffer,MAXBUF,MSG_WAITALL);
				if (tmp < 0)
				{
					error_count++;	
					continue;
				}
				unpack_message(buffer,byte,data);
				if(((LinkC_User_Request *)data)->Flag == 0)
				{
					((LinkC_Sys_Status *)data)->Action = USER_FRIEND_DATA;
					friend_count = get_friends_data (user.UID,&My_friend);
					if (friend_count == 0)		// 如果好友个数为 0
					{
						((LinkC_Sys_Status *)data)->Status = USER_FRIEND_NULL;
						lenth = pack_message(SYS_ACTION_STATUS,data,SYS_STATUS_LENTH,buffer);
						byte = send (sockfd,buffer,lenth,MSG_WAITALL);	// 发送 没有好友
						My_friend = NULL;
						if (byte < 0)		// 如果失败
						{
#if DEBUG
							printf ("Send Error!\n");
#endif
							error_count ++;
						}
						continue;
					}
					else if (friend_count < 0)	// 如果执行失败
					{
						((LinkC_Sys_Status *)data)->Status = SYS_FRIEND_GET_FAILURE;
						lenth = pack_message(SYS_ACTION_STATUS,data,SYS_STATUS_LENTH,buffer);
						byte = send (sockfd,LINKC_ERROR,lenth,MSG_WAITALL);		// 发送 错误
						My_friend = NULL;
						if (byte < 0)
						{
#if DEBUG
							printf ("Send Error!\n");
#endif								error_count ++;
						}
						continue;
					}
					((LinkC_Sys_Status *)data)->Status = SUCCESS;
					lenth = pack_message(SYS_ACTION_STATUS,data,SYS_STATUS_LENTH,buffer);
					byte = send (sockfd,buffer,lenth,MSG_WAITALL);	// 发送 执行成功
#if DEBUG
					printf ("UID = %d\nHave %d friend(s)\n",user.UID,friend_count);
					printf ("------Friends------\n");
					for (i=0;i<friend_count;i++)	printf ("\tUID\t= %d\tNAME\t= %s\n",My_friend[i].UID,My_friend[i].name);
					printf ("------End----------\n");
#endif
					non_std_m_message_send(My_friend,user.sockfd,friend_count,sizeof(friend_data),SYS_FRIEND_DATA,MSG_DONTWAIT);
					free(My_friend);
					My_friend = NULL;
				}
				else
				{
					result = ((LinkC_User_Request *)data)->UID;
					printf("%d\n",result);
					tmp = get_friend_data (user.UID,result,&My_friend);
					printf("%d\n",tmp);
					memcpy(buffer,My_friend,friend_count * sizeof(friend_data));
					buffer[sizeof(friend_data)]='\0';
					lenth = pack_message(SYS_FRIEND_DATA,buffer,sizeof(friend_data),data);
					byte = send (sockfd,data,lenth,MSG_WAITALL);	// 发送好友信息
					free (My_friend);
				}
			}
		} 
	}*/
end:	
#if DEBUG
	printf ("the IP\t=%s closed conncetion!\n",inet_ntoa(user.addr.sin_addr));
#endif
	free(data);
	close(user.sockfd);
	return 0;
}
void cls_buf(char *buffer,int size){
    memset(buffer,'\0',size);
}
