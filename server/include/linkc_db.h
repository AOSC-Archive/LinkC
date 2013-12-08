#ifndef _LINKC_DB_H_
#define _LINKC_DB_H_

#include <sqlite3.h>
#include "linkc_types.h"


extern int init_sqlite3_db (void);
/* 打开数据库，并且设置全局变量 */

extern int check_pass (struct login_data user);
/*
 * 检验用户密码是否正确，并且返回 struct login_data 中的 UID 键值
 * 参数：1	Login_data 结构类型
 */

extern int get_friends_data (int UID,struct friend_data ** ffb);
/*
 * 构建好友的资料，返回值为执行状态，［>0］为成功，［－1］为失败，正常返回值为好友个数
 * 参数：1	int UID		谁的好友，这里的UID为用户唯一的标识
 * 参数：2	struct friend_data * 的地址	运行完成后构建一个指针数组
 */

extern int get_friend_data (int UID, int DestUID,struct friend_data **_friend);
/*
 * 获得单个好友的资料，返回值为得到的资料
 * 参数：1	int UID		UID为用户唯一标识
 * 参数：2	int &STATE	作为返回的状态
 */


extern void linkc_free_data (struct friend_data ** ffb, int _count);
/*
 * 释放由Get_friend_data 所构建的指针，无返回值，并挂空该指针
 * 参数：1	struct friend_data * 的地址
 * 参数：2	Get_friend_data 的返回值 （好友个数）
 */

extern int status_set (struct user_data user,int _Flag);
/*
 * 设置用户的状态
 * 参数：1	int UID		用户的UID
 * 参数：2	int _Sockfd	连接该用户的网络连接句柄
 * 参数：3	int _Flag	操作参数
 */
#define STATE_OK		3
#define STATE_ONLINE		2
#define STATE_HIDING		1
#define STATE_OFFLINE		0
#define STATE_ERROR		-1


extern int get_info (int UID,int _Flag);
/*
 * 获得目标UID用户在线状态	返回[0]表示不在线，[1]表示隐身，[2]表示在线
 * 获得目标UID用户SOCKFD	返回[0]表示没有连接，[-1]表示失败，其余为sockfd
 * 参数：1	int UID		用户的UID
 * 参数：2	int _Flag	操作方式
 */
#define STATE_GET	1
#define SOCKFD_GET	2

extern int add_friend(int UID,int _who_ID,int _Flag);
#define LINKC_FRIEND_ADD			1
#define LINKC_FRIEND_ADD_WITH_NICKNAME		2
#define LINKC_FRIEND_DELETE			3

#define USER_DB_PATH		"./user.db"
#define FRIEND_DB_PATH		"./friend.db"

#endif
