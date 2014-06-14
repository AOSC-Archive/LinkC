#include "../include/linkc_basic_network.h"
#include "../include/linkc_error.h"
#include "../include/linkc_db.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    pid_t LCServer = 0;             // LinkC Server 主进程
    if ((LCServer = fork()) < -1){  //  如果创建新进程失败
        perror("fork");             //      打印错误消息
        exit (EXIT_FAILURE);        //      退出
    }
    else if (LCServer == 0){        //  如果是子进程[即LinkC Server 主进程]
        if(InitNetwork(2341) < 0){
            LinkC_Debug("Init Network",LINKC_FAILURE);
            return -1;
        }
        WaitForConnect();
    }
    return 0;                       //  返回
}
