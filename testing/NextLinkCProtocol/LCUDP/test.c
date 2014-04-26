#include "Protocol/LinkC_NetWorkProtocol.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>

int main(){
    struct sigaction Act;                   //  定义处理信号的参数集合
    sigemptyset(&Act.sa_mask);              //  将数据清空
    Act.sa_sigaction=TimerInt;              //  设置回调函数[请在LinkC_NetWorkProtocol.c中查找]
    Act.sa_flags=SA_SIGINFO;                //  使用sa_sigaction参数的函数最为信号发来后的处理函数[也就是上面定义的]
    InitSocketList();                       //  初始化链表
    struct sockaddr_in addr;
    addr.sin_port = 1023;
    socklen_t   len = sizeof(struct sockaddr_in);
    int Sockfd;
    Sockfd = CreateSocket(AF_INET, (struct sockaddr *)&(addr), len);
    if(sigaction(SIGALRM,&Act,NULL)==-1)    //  安装信号
        printf("install error~!\n");        //      参数[1]-->  信号，这里是时钟信号[后面的第三个参数我就不清楚了]
    alarm(1);                               //  1秒后发射信号
    sleep(2);
    DestroySocketList();
    return 0;
}
