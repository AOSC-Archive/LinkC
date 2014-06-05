/*
 * Author        ： Junfeng Zhang <564691478@qq.com>
 * Last-Change        ： Aprli 5, 2014
 */

#include "../../include/linkc_types.h"
#include "../../include/linkc_db.h"
#include "../../include/linkc_user.h"
#include "../../include/linkc_network.h"
#include "../../include/linkc_network_protocol.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#define DEBUG               1       //  是否为DEBUG模式
#define MAX_ERROR           5       //  最大错误次数
#define MAX_FAILURE_COUNT   5

#define LOGINCOUNT          2       //  最大登录尝试次数
int keep_connect (struct user_data* _user)
{
    int result,tmp,error_count;
    struct user_data user;          //  用户基本数据
    int failure_count = 0;          //  登录已经失败次数
    int byte;                       //  接受的数据。

    struct user_info *info;         //  保存用户信息[诸如用户名，IP等]
    user.sockfd = _user -> sockfd;  //  保存网络句柄
    user.addr = _user -> addr;      //  保存addr

    char  buffer[STD_BUFFER_SIZE];  //  缓存
    void *data = malloc(MAXBUF);    //  同缓存
    uint16_t flag;                  //  用于临时性保存数据包的消息类型
    uint16_t length;                //  保存长度

    if((TCP_Recv(user.sockfd,buffer,STD_BUFFER_SIZE,0)) == LINKC_FAILURE)   // 接受数据
        goto end;
    ((LinkC_Sys_Status *)data)->Action = CONNECTION;        //  将 Action 设置为 CONNECTION
    if(get_message_header(buffer) == CONNECTION)            //  如果先前接受到的数据包的消息头为CONNECTION
        ((LinkC_Sys_Status *)data)->Status = LINKC_SUCCESS; //      将 Status 设置为 LINKC_SUCCESS
    else                                                    //  否则
        goto end;                                           //      关闭链接
    length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer); // 打包数据包
    TCP_Send (user.sockfd,buffer,length,0);                 //  发送数据包
#if DEBUG
    printf ("Socket\t= %d\n",user.sockfd);
    printf ("Connected!\n\tIP\t= %s\n\tPort\t= %d\n",inet_ntoa(user.addr.sin_addr),user.addr.sin_port);    /* 输出连接信息 */
#endif
    failure_count = 0;
start:
    if(TCP_Recv(user.sockfd,buffer,STD_BUFFER_SIZE,0) == LINKC_FAILURE) //  如果接受信息失败
        goto end;                                                       //      关闭链接
    flag = get_message_header(buffer);                                  //  获得消息头
    if(flag == EXIT)    goto end;                                       //  如果为退出请求 则关闭链接
    unpack_message(buffer,data);                                        //  解开数据包
    if (flag == LOGIN)                                                  //  如果是登录请求
    {
        if (failure_count > MAX_FAILURE_COUNT)                          //  如果失败次数大于最大失败次数
        {
            ((LSS *)data)->Action = LOGIN;                              //      |
            ((LSS *)data)->Status = LINKC_LIMITED;                      //      |
            length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer); //      |-> 打包消息 说 登陆超出最大限制
            TCP_Send(user.sockfd,buffer,length,MSG_DONTWAIT);           //      |-> 发送消息
            goto end;                                                   //       -> 关闭链接
        }
        memcpy((void *)&(user.login),data,sizeof(login_data));          //  将之前接受的消息的数据复制为登陆信息
        result = user_login (&user);                                    //  进行登录 , 校验username,password，获取UID
        printf("UID = %d\n",result);
        printf("UserName = %s\nPassWord = %s\n",user.login.user_name,user.login.pass_word);
        ((LSS *)data)->Action = LOGIN;                                  //  设置消息包 Action 为LOGIN
        if (result == LINKC_FAILURE)                                    //  如果登陆结果为 LINKC_FAILURE
        {
            printf ("Login failure!\n");                                //      |
            ((LSS *)data)->Status = LINKC_FAILURE;                      //      |
            length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer); //      |-> 打包消息 说 登陆失败
            TCP_Send(user.sockfd,buffer,length,MSG_DONTWAIT);           //      |-> 发送消息
            failure_count ++;                                           //      |-> 增加错误计数
            goto start;                                                 //       -> 再次等待用户发来的账号密码信息
        }
        printf("Login Success!\n");
        ((LSS *)data)->Status = LINKC_SUCCESS;                          //  否则 设置 Status 为 LINKC_SUCCESS
        length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer);     //  打包消息
        byte = TCP_Send (user.sockfd,buffer,length,0);                  //  发送消息
        error_count = 0;                                                //  初始化错误个数
        while (1)
        {
            if (error_count >= MAX_ERROR)                               // 如果超过最大错误允许范围
            {
#if DEBUG
                printf ("UID = %d cause so much errors!\n",user.UID);
#endif
                user_logout(&user);                                     //      将用户强制登出
                goto end;                                               //      关闭链接
            }
            if(TCP_Recv (user.sockfd,buffer,STD_PACKAGE_SIZE,0) == LINKC_FAILURE)   // 如果在 接收用户发来的数据包 遇到 错误
            {
                error_count ++;                                                     //      增加一个错误计数
                continue;                                                           //      回到循环开始
            }
            flag = get_message_header(buffer);                          //  获得接受接收到的数据包的消息头
            unpack_message(buffer,data);                                //  解包数据包
            if(flag == HEART_BEATS)    continue;                        //  如果消息头为 HEART_BEATS    则 回到循环开头
            if(flag == EXIT)                                            //  如果消息头为 EXIT
            {
                user_logout(&user);                                     //      登出用户
                goto end;                                               //      关闭链接
            }
            else if(flag == USER_REQUEST)                               //  如果消息头为USER_REQUEST
            {
                if(((LUR *)data)->Action == USER_LOGOUT)                //      Request 登出
                {
                    user_logout(&user);                                 //          登出用户
                    goto end;                                           //          关闭链接
                }
                else if(((LUR *)data)->Action == USER_FRIEND_DATA)      //      Request 好友数据
                {
                    if(((LUR *)data)->UID == ALL_FRIENDS)               //          若是获得全部好友数据
                        send_friends_data(user,data);                   //              发送所有好友数据
                    else                                                //          否则
                        send_friend_data(user,data);                    //              发送指定好友数据
                }
                else if(((LUR *)data)->Action == USER_CHAT_REQUEST)     //      Request 聊天
                {
                    if(get_info(((LUR*)data)->UID,STATUS_GET) > 0)      //          指定的好友在线
                    {
                        tmp = get_info(((LUR*)data)->UID,SOCKFD_GET);   //              获取和指定好友连接的网络句柄[求更好的方法]
                        ((LUM *)data)->Action    = USER_CHAT;           //              设置 Action 为 USER_CHAT
                        ((LUM *)data)->SrcUID    = user.UID;            //              设置 SrcUID 为 当前用户的 UID
                        length = pack_message(USER_MESSAGE,data,LUM_L,buffer);//        打包消息
                        send(tmp,buffer,length,0);                      //              向指定好友发送消息
                    }
                }
                else if(((LUR *)data)->Action == USER_INFO_REQUEST)     //      Request 自身的资料
                {
                    ((LSS *)data)->Action = USER_INFO_REQUEST;          //          设置 Action 为获得自己的资料
                    ((LSS *)data)->Status = get_user_info(user.UID,&info);//        设置 Status 为执行 获得自己资料 操作的返回状态
                    length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer);//   打包消息[消息头为SYS_ACTION_STATUS]
                    send(user.sockfd,buffer,length,0);                  //          发送消息[获得自己资料的状态]
                    length = pack_message(SYS_USER_INFO,(void *)info,LUI_L,buffer);//将自己的资料打包[消息头为SYS_USER_INFO]
                    send(user.sockfd,buffer,length,0);                  //          发送消息[自己的资料]
                    free(info);
                }
                else                                                    //      否则
                {
                    printf("This Kind of Header Not Supposed!\n");      //          输出 这个操作不被支持
                }
                continue;
            }
            else if(flag == USER_MESSAGE)                               //  消息头为 USER_MESSAGE
            {
                if(((LUR *)data)->Action == USER_CONNECT_READY)         //      如果 Action 为 USER_CONNECT_READY
                {
                    if(get_info(((LUR*)data)->UID,STATUS_GET) > 0)      //          如果指定用户状态为非下线
                    {
                        tmp = get_info(((LUR*)data)->UID,SOCKFD_GET);   //              获得面向指定好友链接的网络句柄
                        if(tmp == LINKC_FAILURE)                        //              如果失败
                        {
                            error_count ++;                             //                  增加错误计数
                            printf("Get Sockfd Error!\n");
                            continue;                                   //                  回到循环开始
                        }
                        ((LUM *)data)->Action    = USER_CONNECT_READY;  //      设置 Action 为 USER_CONNECT_READY
                        ((LUM *)data)->SrcUID    = user.UID;            //      设置 SrcUID 为 当前用户的 UID
                        length = pack_message(USER_MESSAGE,data,LUM_L,buffer);//打包消息[消息头为USER_MESSAGE]
                        send(tmp,buffer,length,0);                      //      发送给当前用户
                    }
                }
            }
            else                                                        //  如果都没有匹配到
            {
                printf("Message Header Incorrect!\nForce Disconnect!\n");
                error_count = MAX_ERROR;                                //      将错误计数设置为上限[也就是强制断线]
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
