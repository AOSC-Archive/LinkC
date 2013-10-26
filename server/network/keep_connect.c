/* ###################
 * 连接模块[仍在开发中]
   #################### */

#include "linkc_types.h"
#include "linkc_db.h"
#include "linkc_user.h"

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

#define LOGINCOUNT	2		// 最大登录尝试次数
extern void cls_buf(char * buffer,int size);

int msgid;
int keep_connect (struct user_data* _user)
{
	int i,friend_count,result,tmp,error_count;
	struct user_data user;				// 用户基本数据
	int count = 0;					// 已经失败次数
	int byte;					// 接受的数据。
	socklen_t len = sizeof (_user -> addr);

	int sockfd = _user -> sockfd;			// 保存sockfd
	user.sockfd = _user -> sockfd;
	user.addr = _user -> addr;			// 保存addr
	struct friend_data* My_friend = NULL;		// 暂定

	char * body = NULL;
	int DEST_UID;

	char  buffer[MAXBUF + 1];			//缓存

	byte = send (sockfd,LINKC_OK,MAXBUF,MSG_WAITALL);	//发送连接成功
#if DEBUG
	printf ("Socket\t= %d\n",sockfd);
	printf ("Connected!\n\tIP\t= %s\n\tPort\t= %d\n",inet_ntoa(user.addr.sin_addr),user.addr.sin_port);	/* 输出连接信息 */
#endif
	while(1)
	{
		byte = recv (sockfd,buffer,MAXBUF,MSG_WAITALL);
		if (byte <= 0)	goto end;
		if (!strncasecmp(buffer,LINKC_QUIT,MAXBUF))
		{
			printf ("Recv:QUIT\n");
			goto end;
		}
		if (!strncasecmp(buffer,LINKC_LOGIN,MAXBUF))	/* 如果是登录请求 */
		{
			cls_buf (buffer,MAXBUF);
			byte = recv (sockfd,buffer,MAXBUF,MSG_WAITALL);
			memcpy((void *)&user.login,buffer,sizeof(struct login_data));
			if (byte <= 0)	goto end;
			
			result = user_login (&user);		/* 进行登录 , 获得username,password和UID*/
			if (result < 0)		goto end;		// 如果出现错误，则退出
			else if(result == 0)			// 如果登陆失败，则增加错误计数，然后continue
			{
				count++;
				printf ("Login failure!\n");
				send(user.sockfd,LINKC_FAILURE,MAXBUF,MSG_DONTWAIT);
				if (count > LOGINCOUNT)
               			{
					byte = send (sockfd,LINKC_TRY_SO_MANY,500,MSG_DONTWAIT);
#if DEBUG
					printf ("You tried so many times!\n");
#endif
				}
				continue;
			}

			byte = send (user.sockfd,LINKC_OK,MAXBUF,MSG_WAITALL);
			error_count = 0;			// 初始化错误个数
			while (1)				// 如果成功
			{
				if (error_count > MAX_ERROR)		// 如果超过最大错误允许范围
				{
#if DEBUG
					printf ("UID = %d cause so much error!\n",user.UID);
#endif
					goto end;
				}
				byte = recv (user.sockfd,buffer,MAXBUF,MSG_WAITALL);
				if (byte <= 0)				// 如果接受错误，则增加一个错误计数
				{
					printf ("Recv Nothing!\n");
					error_count ++;
					continue;
				}
#if DEBUG
				sleep(1);
				printf ("RECV %s\n",buffer);				// 输出接受信息
#endif
				if (!strncasecmp (buffer,LINKC_LOGOUT,MAXBUF))	//如果接受数据为 注销
					break;
				if (!strncasecmp (buffer,LINKC_GET_FRIEND,MAXBUF))	//如果接受数据为 请求单个好友数据
				{
					cls_buf (buffer,MAXBUF);
					tmp = recv(user.sockfd,buffer,MAXBUF,MSG_WAITALL);
					if (tmp < 0)
					{
						error_count++;	
						continue;
					}
					result = atoi(buffer);
					printf("%d\n",result);
					tmp = get_friend_data (user.UID,result,&My_friend);
					printf("%d\n",tmp);
					memcpy(buffer,My_friend,friend_count * sizeof(friend_data));
					buffer[sizeof(friend_data)]='\0';
					byte = send (sockfd,buffer,MAXBUF,MSG_WAITALL);	// 发送好友信息
					free (My_friend);
				}
				if (!strncasecmp (buffer,LINKC_GET_FRIENDS,MAXBUF))	// 如果接受数据为 请求好友数据
				{
					friend_count = get_friends_data (user.UID,&My_friend);
					if (friend_count == 0)		// 如果好友个数为 0
					{
						byte = send (sockfd,LINKC_NO_FRIEND,MAXBUF,MSG_WAITALL);	// 发送 没有好友
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
						byte = send (sockfd,LINKC_ERROR,MAXBUF,MSG_WAITALL);		// 发送 错误
						My_friend = NULL;
						if (byte < 0)
						{
#if DEBUG
							printf ("Send Error!\n");
#endif
							error_count ++;
						}
						continue;
					}
					byte = send (sockfd,LINKC_OK,MAXBUF,MSG_WAITALL);	// 发送 执行成功
#if DEBUG
					printf ("UID = %d\nHave %d friend(s)\n",user.UID,friend_count);
					printf ("------Friends------\n");
					for (i=0;i<friend_count;i++)	printf ("\tUID\t= %d\tNAME\t= %s\n",My_friend[i].UID,My_friend[i].name);
					printf ("------End----------\n");
#endif
					sprintf (buffer,"%d",friend_count);
					byte = send(user.sockfd,buffer,MAXBUF,MSG_WAITALL);
					memcpy(buffer,My_friend,friend_count * sizeof(friend_data));
					buffer[sizeof(friend_data)*friend_count]='\0';
					byte = send (sockfd,buffer,MAXBUF,MSG_WAITALL);	// 发送好友信息
					free(My_friend);
					My_friend = NULL;
					if (byte < 0)
					{
#if DEBUG
							printf ("Send Error!\n");
#endif	
							error_count ++;
							continue;
					}
				}
				if (!strncasecmp(buffer,LINKC_QUIT,MAXBUF))	goto end;	// 退出
				if (!strncasecmp (buffer,LINKC_CHAT_WANT,MAXBUF))	//如果是聊天请求
				{
					byte = send (sockfd,"Who?",MAXBUF,MSG_WAITALL);
					byte = recv (sockfd,buffer,MAXBUF,MSG_WAITALL);
					sprintf (buffer,"%d",DEST_UID);			// 获得目标的UID
					i = chat_with(user.UID,DEST_UID,sockfd);	// 执行并获得想发送的数据，然后发送
					if (i == -1)
					{
						error_count++;
						continue;
					}
				}
			} 
		}
	}
end:	
#if DEBUG
	printf ("the IP\t=%s closed conncetion!\n",inet_ntoa(user.addr.sin_addr));
#endif
	user_logout(user);
	return 0;
}
void cls_buf(char *buffer,int size){
    memset(buffer,'\0',size);
}
