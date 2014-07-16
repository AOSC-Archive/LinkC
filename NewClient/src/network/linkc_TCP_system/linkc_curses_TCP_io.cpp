#include "../../../include/linkc_TCP_system/linkc_curses_TCP_io.h"
#include "../../../include/linkc_def.h"
#include "../../../include/linkc_curses_error.h"
#include "../../../include/linkc_package.h"
#include "../../../include/linkc_client_nonui.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <memory.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <curses.h>

char    recv_buffer[MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1];    // 接收缓冲区
char    Tmp[MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1];        // 临时缓冲区
int    is_remain    = 0;    // 上次数据是否有剩余
int    Length         = 0;    // 接收到数据的长度
int    tmp;
uint16_t    TmpLength;


int16_t wTCP_Recv(WINDOW* Console,int Sockfd, void *Out, int Out_size, int flag){
    PackageHeader Header;
    if(recv(Sockfd,(void*)&Header,sizeof(PackageHeader),MSG_PEEK) == LINKC_FAILURE)
        return LINKC_FAILURE;
    int PackageLength = ntohs(Header.MessageLength)+sizeof(PackageHeader);
    if(PackageLength > Out_size){
        wLinkC_Debug(Console,"Send-Out Buffer Too Small",LINKC_FAILURE);
        return LINKC_FAILURE;
    }
    int NowRecv = 0;
    int TmpSize = 0;
    while(1){
        TmpSize = recv(Sockfd,(char*)Out+NowRecv,PackageLength - NowRecv,flag);
        if(TmpSize <= 0){
            wLinkC_Debug(Console,"Accepted Data",LINKC_FAILURE);
            return LINKC_FAILURE;
        }
        NowRecv += TmpSize;
        if(NowRecv == PackageLength)
            break;
    }
    return LINKC_SUCCESS;
}
