/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： March 22, 2014
 */

#ifndef LINKC_TYPES_H
#define LINKC_TYPES_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include "Def/LinkC_Error.h"

typedef uint32_t	id_t;
typedef uint32_t    ip_t;
typedef uint32_t    port_t;

typedef struct login_data
{
        char user_name[12];
        char pass_word[20];
}login_data;

struct LinkC_User_Info_t                // 用户信息，用于发回数据
{
        id_t UID;
        char name[20];                  // 自己定义的昵称
        char username[20];              // 用户名
        char telephone[20];             // 电话
        char company[20];               // 公司
        char address[80];               // 地址
        char join_time[25];             // 创建时间
        char last_login[25];            // 上次登陆时间
        int status;                     // 现在状态
        ip_t ip;                        // 现在IP地址
};

typedef struct LinkC_Friend_Data_t
{
	char nickname[20];
	int group;
    struct LinkC_User_Info_t Info;
}friend_data;

#endif



