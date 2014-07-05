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

int Login(int Sockfd,LoginData Data){
    void *Buffer    = malloc(STD_PACKAGE_SIZE);
    void *Package   = malloc(STD_PACKAGE_SIZE);
    ((MessageHeader*)Buffer)->ActionType = USER_LOGIN;
    memcpy((void*)((char*)Buffer+4),(void*)&Data,sizeof(LoginData));
    int Length = _Package(Buffer,sizeof(MessageHeader)+sizeof(LoginData),NORMAL_MESSAGE,Package);
    printf("Length = %d\n",Length);
    printf("Length = %d\n",ntohs(((PackageHeader*)Package)->MessageLength));
    send(Sockfd,Package,Length,0);
    LinkC_Debug("LoginRequest Sent",LINKC_DEBUG);
    TCP_recv(Sockfd,Buffer,STD_PACKAGE_SIZE,0);
    LinkC_Debug("接收",LINKC_SUCCESS);
    if(_UnPackage(Buffer,STD_PACKAGE_SIZE,Package) == LINKC_FAILURE)
        return LINKC_FAILURE;
    if(ntohs(((MessageHeader*)Package)->StatusCode) == LOGIN_FAILURE)
        return LINKC_FAILURE;
    free(Buffer);
    free(Package);
    return LINKC_SUCCESS;
}
