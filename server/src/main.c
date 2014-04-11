/*
 * Author        ： Junfeng Zhang <564691478@qq.com>
 * Last-Change        ： April 6, 2014
 */
#include "linkc_network.h"
#include "p2p_helper.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
#if PER_USER_TEST == 1
    start_connect();
#else
    pid_t LCServer = 0;             // LinkC Server
    pid_t P2PServer = 0;            // P2P Helper
    if ((LCServer = fork()) < -1)   // 创建新进程失败
    {
        perror("fork");             //      打印错误消息
        exit (EXIT_FAILURE);        //      退出
    }
    else if (LCServer == 0)         //  如果是子进程
    {
        start_connect();            //      执行start_connect()函数[见network/init_network.h]
    }
    if ((P2PServer = fork()) < -1)  //  创建新进程失败
    {
        perror("fork");             //      输出错误信息
        exit (EXIT_FAILURE);        //      退出
    }
    else if (P2PServer == 0)        //  如果是子进程
    {
        p2p_helper();               //      执行 p2p_helper()函数
    }
    else        getchar();          //  getchar 暂停父进程
#endif
    printf("Quit!\n");
    exit (0);
}
