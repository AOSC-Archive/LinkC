/* ###########################
 * # 线程内内部通信[暂时不用]
 * ###########################*/

#include "../include/server_data_type.h"
#include "../include/linkc_db.h"
#include "../include/linkc_qmsg.h"
#include <stdlib.h>
int qmsg(int msgid)
{
	printf ("QMSG INIT!\n");
	struct msg_type recv_msg;
	int sockfd;
	int byte;
	char buffer [MAXBUF];
	while (1)
	{
		if (msgrcv (msgid,(void *)&recv_msg,MAXBUF,0,0) == -1)
		{
			printf ("MsgRcv Error!\n");
			pthread_exit(0);
		}
		sockfd = get_info (recv_msg.DEST_UID,SOCKFD_GET);
		if (sockfd < 0)
		{
			printf ("Get Sockfd Error!\n");
			exit (EXIT_FAILURE);
		}
		sprintf (buffer,"A_Message_From_ID_%d",recv_msg.SCR_UID);
		byte = send (sockfd,buffer,MAXBUF,MSG_WAITALL);
		byte = send (sockfd,recv_msg.msg_text,MAXBUF,MSG_WAITALL);
		if (byte < 0)
		{
			printf ("Send Error!\n");
			exit (EXIT_FAILURE);
		}
		sockfd = 0;
	}
}
