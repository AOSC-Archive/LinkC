#include "../../include/linkc_client.h"
#include "../../include/linkc_error.h"
#include "../../include/linkc_client_nonui.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

int NonUiMode(){
    LinkC_Debug("Non-UI-Mode",LINKC_DEBUG);
    int Sockfd = InitNetwork();
    if(ConnectToServer(Sockfd) == LINKC_FAILURE)    return -1;
    if(NonUiLogin(Sockfd) == LINKC_FAILURE){
        LinkC_Debug("Login",LINKC_FAILURE);
        return LINKC_FAILURE;
    }
    LinkC_Debug("Login",LINKC_SUCCESS);
    getchar();
    return 0;
}

int NonUiLogin(int Sockfd){
    LoginData Data;
    bzero((void*)&Data,sizeof(LoginData));
    printf("UserName:");
    scanf("%s",(char*)&(Data.UserName));
    printf("PassWord:");
    scanf("%s",(char*)&(Data.PassWord));
    return Login(Sockfd,Data);
}
