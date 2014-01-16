/* ###################
 * 连接模块[仍在开发中]
   #################### */

#include "linkc_types.h"
#include "linkc_db.h"
#include "linkc_user.h"
#include "linkc_network.h"
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
	int result,tmp,error_count,count,friend_count;
	struct user_data user;				// 用户基本数据
	int failure_count = 0;				// 登录已经失败次数
	int byte;					// 接受的数据。

	int sockfd = _user -> sockfd;			// 保存sockfd
	user.sockfd = _user -> sockfd;
	user.addr = _user -> addr;			// 保存addr
	struct friend_data* My_friend = NULL;		// 暂定

	char  buffer[MAXBUF];			//缓存
	void *data = malloc(MAXBUF);
	uint16_t flag;
	uint16_t offset=0;
	uint16_t length;


	byte = recv(user.sockfd,buffer,STD_PACKAGE_SIZE,0);
	tmp = check_message(buffer,byte);
	((LinkC_Sys_Status *)data)->Action = CONNECTION;
	if(check_message(buffer,byte) == CONNECTION)
		((LinkC_Sys_Status *)data)->Status = LINKC_SUCCESS;
	else
		((LinkC_Sys_Status *)data)->Status = LINKC_FAILURE;
	length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer);
	byte = send (sockfd,buffer,length,0);
#if DEBUG
	printf ("Socket\t= %d\n",sockfd);
	printf ("Connected!\n\tIP\t= %s\n\tPort\t= %d\n",inet_ntoa(user.addr.sin_addr),user.addr.sin_port);	/* 输出连接信息 */
#endif
	failure_count = 0;
start:
	byte = recv (sockfd,buffer+offset,STD_PACKAGE_SIZE,0);
	if (byte <= 0)		goto end;
	flag = check_message(buffer,byte+offset);
	if(flag == NOT_MESSAGE || flag == EXIT)	goto end;	/* 不是消息或者是退出请求 */
	if(flag == MESSAGE_INCOMPLETE)
	{
		offset += byte;					/* 设置偏移量 */
		if(offset > 1024)	goto end;
		goto start;
	}
	else	offset = 0;
	unpack_message(buffer,byte,data);
	if (flag == LOGIN)	/* 如果是登录请求 */
	{
		if (failure_count > MAX_FAILURE_COUNT)
		{
			((LinkC_Sys_Status *)data)->Action = LOGIN;
			((LinkC_Sys_Status *)data)->Status = LINKC_LIMITED;
			length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer);
			send(user.sockfd,buffer,length,MSG_DONTWAIT);
			goto end;
		}
		memcpy((void *)&(user.login),data,sizeof(login_data));
		result = user_login (&user);	/* 进行登录 , 获得username,password和UID*/
		printf("Result = %d\n",result);
		printf("UserName = %s\nPassWord = %s\n",user.login.user_name,user.login.pass_word);
		((LinkC_Sys_Status *)data)->Action = LOGIN;
	        if (result == LINKC_FAILURE)
		{
			printf ("Login failure!\n");
			((LinkC_Sys_Status *)data)->Status = LINKC_FAILURE;
			length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer);
			send(user.sockfd,buffer,length,MSG_DONTWAIT);
			failure_count ++;
			goto start;
		}
		printf("Login Success!\n");
		((LinkC_Sys_Status *)data)->Status = LINKC_SUCCESS;
		length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer);
		byte = send (user.sockfd,buffer,length,0);
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
			byte = recv (user.sockfd,buffer+offset,STD_PACKAGE_SIZE,0);
			if (byte <= 0)				// 如果接受错误，则增加一个错误计数
			{
				printf ("Recv Nothing!\n");
				error_count ++;
				continue;
			}
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
			switch(flag)
			{
				case HEART_BEATS:	continue;
				case USER_REQUEST:
				{
					if(((LinkC_User_Request *)data)->Action == USER_LOGOUT)	// 注销
					{
						user_logout(user);
						goto end;
					}
					else if(((LinkC_User_Request *)data)->Action == USER_FRIEND_DATA)	// 好友数据
					{
						if(((LinkC_User_Request *)data)->Flag == ALL_FRIEND)		// 若是获得全部好友数据
						send_friends_data(user,data);
					}
					else if(((LinkC_User_Request *)data)->Action == USER_LOGOUT)
					{
						user_logout(user);
						goto end;
					}
				}
			}
		}
	}
end:
#if DEBUG
	printf ("the IP\t=%s closed conncetion!\n",inet_ntoa(user.addr.sin_addr));
#endif
	free(data);
	close(user.sockfd);
	return 0;
}
