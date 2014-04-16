#include "LinkC_NetWorkProtocol.h"
#include <unistd.h>
#include <stdio.h>

int main(){
    struct sigaction Act;                   //  定义处理信号的参数集合
    sigemptyset(&Act.sa_mask);              //  设置掩码[不明白]
    Act.sa_sigaction=TimerInt;              //  设置回调函数[请在LinkC_NetWorkProtocol.c中查找]
    Act.sa_flags=SA_SIGINFO;                //  设置Flag[不明白]
                    
    if(sigaction(SIGALRM,&Act,NULL)==-1)    //  安装信号
        printf("install error~!\n");        //      参数[1]-->  信号，这里是时钟信号[后面的第三个参数我就不清楚了]
    alarm(1);                               //  1秒后发射信号
    while(1){};
    return 0;
}
