/*  LinkC_Server Only Functions */
#include "../../include/linkc_server.h"
#include "../../include/linkc_def.h"
#include "../../include/linkc_error.h"
#include "../../include/linkc_db.h"
#include "../../include/linkc_package_ctl.h"
#include "../../include/linkc_package.h"
#include <stdint.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <strings.h>

uint8_t GetActionType(uint8_t Type){
    return Type&0xF0;
}

uint8_t GetDataType(uint8_t Type){
    return Type&0x0F;
}
int ReplyData (UserData* User,int Sockfd,uint8_t Request){
    uint8_t DataType = GetDataType(Request);
    if(DataType == SELF_DATA){
        if(GetUserData(User->UID,User->UID,User) == LINKC_FAILURE){
            LinkC_Debug("GetUserData",LINKC_FAILURE);
            SendActionStatus(Sockfd,GET_DATA_FAILURE);
            return LINKC_FAILURE;
        }
        void *Buffer = malloc(sizeof(MessageHeader)+sizeof(UserData));
        void *Package =malloc(STD_PACKAGE_SIZE);
        bzero(Buffer,sizeof(MessageHeader)+sizeof(UserData));
        bzero(Package,STD_PACKAGE_SIZE);
        ((MessageHeader*)Buffer)->ActionType = RETURN_DATA|USER_DATA;
        ((MessageHeader*)Buffer)->StatusCode = htons(GET_DATA_SUCCESS);
        int Length = _Package(Buffer,sizeof(MessageHeader)+sizeof(UserData),NORMAL_MESSAGE,Package);
        send(Sockfd,Package,Length,0);
        free(Buffer);
        free(Package);
        Buffer = NULL;
        Package = NULL;
        return LINKC_FAILURE;
    }else{
        LinkC_Debug("返回数据：操作不被支持",LINKC_WARNING);
        return LINKC_FAILURE;
    }
    return LINKC_SUCCESS;
}
