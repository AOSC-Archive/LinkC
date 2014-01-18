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

	user.sockfd = _user -> sockfd;
	user.addr = _user -> addr;			// 保存addr
	struct friend_data* My_friend = NULL;		// 暂定

	char  buffer[STD_BUFFER_SIZE];			//缓存
	void *data = malloc(MAXBUF);
	uint16_t flag;
	uint16_t length;

	if((TCP_Recv(user.sockfd,buffer,STD_BUFFER_SIZE,0)) == LINKC_FAILURE)
		goto end;
	((LinkC_Sys_Status *)data)->Action = CONNECTION;
	if(get_message_header(buffer) == CONNECTION)
		((LinkC_Sys_Status *)data)->Status = LINKC_SUCCESS;
	else
		((LinkC_Sys_Status *)data)->Status = LINKC_FAILURE;
	length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer);
	TCP_Send (user.sockfd,buffer,length,0);
#if DEBUG
	printf ("Socket\t= %d\n",user.sockfd);
	printf ("Connected!\n\tIP\t= %s\n\tPort\t= %d\n",inet_ntoa(user.addr.sin_addr),user.addr.sin_port);	/* 输出连接信息 */
#endif
	failure_count = 0;
start:
	if(TCP_Recv(user.sockfd,buffer,STD_BUFFER_SIZE,0) == LINKC_FAILURE)
		goto end;
	flag = get_message_header(buffer);
	if(flag == EXIT)	goto end;	/* 退出请求 */
	unpack_message(buffer,data);
	if (flag == LOGIN)	/* 如果是登录请求 */
	{
		if (failure_count > MAX_FAILURE_COUNT)
		{
			((LSS *)data)->Action = LOGIN;
			((LSS *)data)->Status = LINKC_LIMITED;
			length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer);
			TCP_Send(user.sockfd,buffer,length,MSG_DONTWAIT);
			goto end;
		}
		memcpy((void *)&(user.login),data,sizeof(login_data));
		result = user_login (&user);	/* 进行登录 , 获得username,password和UID*/
		printf("Result = %d\n",result);
		printf("UserName = %s\nPassWord = %s\n",user.login.user_name,user.login.pass_word);
		((LSS *)data)->Action = LOGIN;
	        if (result == LINKC_FAILURE)
		{
			printf ("Login failure!\n");
			((LSS *)data)->Status = LINKC_FAILURE;
			length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer);
			TCP_Send(user.sockfd,buffer,length,MSG_DONTWAIT);
			failure_count ++;
			goto start;
		}
		printf("Login Success!\n");
		((LSS *)data)->Status = LINKC_SUCCESS;
		length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer);
		byte = TCP_Send (user.sockfd,buffer,length,0);
		error_count = 0;	/* 初始化错误个数 */
		while (1)
		{
			if (error_count > MAX_ERROR)		// 如果超过最大错误允许范围
			{
#if DEBUG
				printf ("UID = %d cause so much errors!\n",user.UID);
#endif
				user_logout(&user);
				goto end;
			}
			if(TCP_Recv (user.sockfd,buffer,STD_PACKAGE_SIZE,0) == LINKC_FAILURE)	// 如果接受错误，则增加一个错误计数
			{
				error_count ++;
				continue;
			}
			flag = get_message_header(buffer);
			unpack_message(buffer,data);
			if(flag == HEART_BEATS)	continue;
			if(flag == EXIT)
			{
				user_logout(&user);
				goto end;
			}
			else if(flag == USER_REQUEST)
			{
				if(((LUR *)data)->Action == USER_LOGOUT)	// 注销
				{
					user_logout(&user);
					goto end;
				}
				else if(((LUR *)data)->Action == USER_FRIEND_DATA)	// 好友数据
				{
					if(((LUR *)data)->Flag == 0)		// 若是获得全部好友数据
						send_friends_data(user,data);
					else
						send_friend_data(user,data);
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
