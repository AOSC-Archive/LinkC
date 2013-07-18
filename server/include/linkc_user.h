#ifndef _LINKC_USER_H_
#define _LINKC_USER_H_

extern int user_login(struct user_data* data);
/* 
 * 用户登录，成功返回0,失败返回-1，同时向客户端发送登录信息
 * 参数：1	strcut user_data 的地址
 */

extern int user_logout(int UID);
/*
 * 用户注销，成功返回0,失败返回-1.
 * 参数：1	int UID		用户的UID
 * 参数：2	int sockfd	用户的Sockfd
 */
extern int chat_with  (int SRC_UID,int DEST_UID,int sockfd);

#endif
