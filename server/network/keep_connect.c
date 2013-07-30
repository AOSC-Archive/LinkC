/* ###################
 * 连接模块[仍在开发中]
   #################### */

#include "../include/server_data_type.h"
#include "../include/linkc_db.h"
#include "../include/linkc_user.h"

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
#define QMSG		0		// 是否启用消息队列
#define MAX_ERROR	5		// 最大错误次数
#define STRSEP		0		// 是否启用STRSEP

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


#if STRSEP
	char *value = NULL;
	char *ch = NULL;
#else
	char * body = NULL;
#endif

#if QMSG
	struct msg_type Send_msg;
#else
	int DEST_UID;
#endif

	

	char  buffer[MAXBUF + 1];			//缓存

	byte = send (sockfd,LINKC_OK,MAXBUF,MSG_WAITALL);	//发送连接成功
#if DEBUG
	printf ("Socket\t= %d\nMsgid\t= %d\n",sockfd,msgid);
	printf ("Connected!\n\tIP\t= %s\n\tPort\t= %d\n",inet_ntoa(user.addr.sin_addr),user.addr.sin_port);	/* 输出连接信息 */
#endif
	while(1)
	{
		byte = recv (sockfd,buffer,MAXBUF,MSG_WAITALL);
		if (byte <= 0)	goto end;
#if STRSEP
		ch = buffer;
		value = strsep(&ch,"&");

		if (!strncasecmp(value,LINKC_QUIT,MAXBUF))
#else
		if (!strncasecmp(buffer,LINKC_QUIT,MAXBUF))
#endif
		{
			printf ("Recv:QUIT\n");
			goto end;
		}
#if STRSEP
		if (!strncasecmp(value,LINKC_LOGIN,MAXBUF))	/* 如果是登录请求 */
		{
			value=strsep(&ch,"&");			// 拆分字符串
			strncpy(user.login.user_name,value,sizeof(user.login.user_name));	// 得到帐号
			value=strsep(&ch,"&");			// 拆分字符串
			strncpy(user.login.pass_word,value,sizeof(user.login.pass_word));	// 得到密码
#else
		if (!strncasecmp(buffer,LINKC_LOGIN,MAXBUF))	/* 如果是登录请求 */
		{
			cls_buf (buffer,MAXBUF);
			byte = recv (sockfd,buffer,MAXBUF,MSG_WAITALL);
			memcpy((void *)&user.login,buffer,sizeof(struct login_data));
			if (byte <= 0)	goto end;
			
#endif
			result = user_login (&user);		/* 进行登录 , 获得username,password和UID*/
			if (result < 0)		goto end;		// 如果出现错误，则退出
			else if(result == 0)			// 如果登陆失败，则增加错误计数，然后continue
			{
				count++;
				printf ("Login failure!\n");
				send(user.sockfd,LINKC_FAILURE,1024,MSG_DONTWAIT);
				if (count > LOGINCOUNT)
               			{
					byte = send (sockfd,LINKC_TRY_SO_MANY,500,MSG_DONTWAIT);
#if DEBUG
					printf ("You tried so many times!\n");
#endif
				}
				continue;
			}

			byte = send (user.sockfd,LINKC_OK,1024,MSG_WAITALL);
			error_count = 0;			// 初始化错误个数
			while (1)				// 如果成功
			{
				if (error_count > MAX_ERROR)		// 如果超过最大错误允许范围
				{
#if DEBUG
					printf ("UID = %d cause so much error!\n",user.UID);
#endif
//					user_logout(user.UID);
//					byte = send (sockfd,LINKC_ERROR,MAXBUF,MSG_DONTWAIT);		// 发送 ERROR 信息
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
				{
					user_logout(user.UID);	// 执行注销
					break;
				}
				if (!strncasecmp (buffer,LINKC_GET_FRIEND,MAXBUF))	// 如果接受数据为 请求好友数据
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
					byte = send (sockfd,buffer,1024,MSG_WAITALL);	// 可能有错误
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
	return 0;
}
void cls_buf(char *buffer,int size){
    memset(buffer,'\0',size);
}
