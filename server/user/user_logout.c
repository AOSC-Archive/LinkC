/* ###################################################
 * # 注销一个用户	参数1 [用户UID]	参数2 [用户连接的Sockfd]
 * ###################################################*/


#include "../include/server_data_type.h"
#include "../include/linkc_db.h"
#include "../include/linkc_user.h"
#define DEBUG 1
int user_logout(int UID)
{
	int i,byte;
	i = state_set (UID,0,STATE_OFFLINE);	// 设置下线
	if (i == STATE_ERROR)
	{
/*		byte = send (sockfd,LINKC_ERROR,MAXBUF,MSG_WAITALL);
		if (byte < 0)
		{
#if DEBUG
			printf ("Send Error!\n");
#endif
		}
		byte = send (sockfd,LINKC_OK,MAXBUF,MSG_WAITALL);
		if (byte < 0)		// 如果失败
		{
#if DEBUG
			printf ("Send Error!\n");
#endif
		}*/
		return -1;
	}
	return 0;
}
