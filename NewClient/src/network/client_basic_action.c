#include "../../include/linkc_client.h"
#include "../../include/linkc_TCP_system/linkc_TCP_io.h"
#include "../../include/linkc_error.h"
#include "../../include/linkc_def.h"
#include "../../include/linkc_package.h"
#include "../../include/linkc_package_ctl.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <string.h>
#include <curses.h>

int Login(int Sockfd,LoginData Data){
    void *Buffer    = malloc(STD_PACKAGE_SIZE);
    void *Package   = malloc(STD_PACKAGE_SIZE);
    ((MessageHeader*)Buffer)->ActionType = USER_LOGIN;
    memcpy((void*)((char*)Buffer+4),(void*)&Data,sizeof(LoginData));
    int Length = _Package(Buffer,sizeof(MessageHeader)+sizeof(LoginData),NORMAL_MESSAGE,Package);
    send(Sockfd,Package,Length,0);
    wTCP_Recv(Sockfd,Buffer,STD_PACKAGE_SIZE,0);
    if(_UnPackage(Buffer,STD_PACKAGE_SIZE,Package) == LINKC_FAILURE)
        return LINKC_FAILURE;
    if(ntohs(((MessageHeader*)Package)->StatusCode) == LOGIN_FAILURE)
        return LINKC_FAILURE;
    wLinkC_Debug("用户密码检验",LINKC_SUCCESS);

    wTCP_Recv(Sockfd,Buffer,STD_PACKAGE_SIZE,0);
    if(_UnPackage(Buffer,STD_PACKAGE_SIZE,Package) == LINKC_FAILURE)
        return LINKC_FAILURE;
    if(ntohs(((MessageHeader*)Package)->StatusCode) == SET_STATUS_FAILURE)
        return LINKC_FAILURE;
    wLinkC_Debug("状态设置",LINKC_SUCCESS);

    free(Buffer);
    free(Package);
    return LINKC_SUCCESS;
}
