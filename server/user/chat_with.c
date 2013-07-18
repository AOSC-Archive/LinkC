/* ##################################################
 * # 发送聊天信息给指定用户
 * ################################################## */
#include "../include/server_data_type.h"
#include "../include/linkc_db.h"
#include "../include/linkc_user.h"

int chat_with(int SRC_UID,int DEST_UID,int sockfd)
{
	int byte,i;
	if (SRC_UID == DEST_UID)		// 如果目标UID与自身UID相同，返回错误
	{
		byte = send (sockfd,LINKC_ERROR,MAXBUF,MSG_WAITALL);
		if (byte == -1)
		{
#if DEBUG
			printf ("Send Error\n");
#endif
		}
		return -1;
	}
	int dest_sockfd = 0;
	char msg_text [1024];

	i = get_info(DEST_UID,STATE_GET);
	if (i == -1)				// 如果获取失败
	{
		byte = send (sockfd,LINKC_ERROR,MAXBUF,MSG_WAITALL);
		return -1;
	}
	else if (i == 0)			// 如果好友不在线
	{
#if DEBUG
		printf ("Is not Online!\n");
#endif
		return 0;
	}
	dest_sockfd = get_info (DEST_UID,SOCKFD_GET);
	if (dest_sockfd == -1)				// 如果获取失败
	{
		byte = send (sockfd,LINKC_ERROR,MAXBUF,MSG_WAITALL);
		return -1;
	}
	byte = send (sockfd,LINKC_OK,MAXBUF,MSG_WAITALL);
	if (byte < 0)				// 如果发送失败
	{
#if DEBUG
		printf ("Send Error!\n");
#endif
		return -1;
	}
	byte = send (sockfd,"What?",MAXBUF,MSG_WAITALL);
	byte = recv (sockfd,msg_text,MAXBUF,MSG_WAITALL);
	byte = send (dest_sockfd,msg_text,MAXBUF,MSG_WAITALL);	//向目标发送信息
	if (byte == -1)
	{
#if DEBUG
		printf ("Msgsnd Error\n");
#endif
		return -1;
	}
							
#if DEBUG
	printf ("-----Chat_Want------\nSCR\t= %d\nDEST\t= %d\n-----END--------\n",Send_msg.SCR_UID,Send_msg.DEST_UID);
#endif
	return 1;	
}
