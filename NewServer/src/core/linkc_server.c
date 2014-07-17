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
#include <string.h>

uint8_t GetActionType(uint8_t Type){
    return Type&0xF0;
}

uint8_t GetDataType(uint8_t Type){
    return Type&0x0F;
}
int ReplyData (UserData* User,int Sockfd,uint8_t Request, RequestUser* Dest){
    int Length;
    void *Buffer = malloc(sizeof(MessageHeader)+sizeof(UserData));
    void *Package =malloc(STD_PACKAGE_SIZE);
    bzero(Buffer,sizeof(MessageHeader)+sizeof(UserData));
    bzero(Package,STD_PACKAGE_SIZE);
    if(Request == SELF_DATA){
        if(GetUserData(User->UID,User->UID,User) == LINKC_FAILURE){
            LinkC_Debug("Accessing Databse [1]",LINKC_FAILURE);
            SendActionStatus(Sockfd,GET_DATA_FAILURE);
            goto RETURN_FAILURE;
        }
        memcpy((char*)Buffer+sizeof(MessageHeader),(void*)User,sizeof(UserData));
        ((MessageHeader*)Buffer)->ActionType = RETURN_DATA|SELF_DATA;
        ((MessageHeader*)Buffer)->StatusCode = htons(GET_DATA_SUCCESS);
        Length = _Package(Buffer,sizeof(MessageHeader)+sizeof(UserData),NORMAL_MESSAGE,Package);
        send(Sockfd,Package,Length,0);
        User->UID = (ntohl(User->UID));
        goto RETURN_SUCCESS;
    }else if(Request == USER_DATA){
        if(Dest->UID == 0){             //  获得全部好友数据
            UserData* Friends = NULL;
            int Count = GetFriendsData(User->UID,&Friends);
            if(Count == LINKC_FAILURE){
                LinkC_Debug("Fetching Friend Data",LINKC_FAILURE);
                goto RETURN_FAILURE;
            }else if(Count == 0){
                LinkC_Debug("No Friend Data Available",LINKC_DEBUG);
                SendActionStatus(Sockfd,NO_DATA);
                goto RETURN_SUCCESS;
            }
            free(Package);
            free(Buffer);
            Buffer  = malloc(sizeof(MessageHeader)+Count*sizeof(UserData));
            Package = malloc(sizeof(PackageHeader)+sizeof(MessageHeader)+Count*sizeof(UserData));
            ((MessageHeader*)Buffer)->ActionType = RETURN_DATA|FRIENDS_DATA;
            ((MessageHeader*)Buffer)->StatusCode = htons(GET_DATA_SUCCESS);
            memcpy((char*)Buffer+sizeof(MessageHeader),Friends,Count*sizeof(UserData));
            Length = _Package(Buffer,sizeof(MessageHeader)+Count*sizeof(UserData),NORMAL_MESSAGE,Package);
            send(Sockfd,Package,Length,0);
            goto RETURN_SUCCESS;
        }else{
            if(GetUserData(User->UID,ntohl(Dest->UID),(UserData*)(char*)Buffer+sizeof(MessageHeader)) == LINKC_FAILURE){
                LinkC_Debug("Accessing Database [2]",LINKC_FAILURE);
                SendActionStatus(Sockfd,GET_DATA_FAILURE);
                goto RETURN_FAILURE;
            }
            ((MessageHeader*)Buffer)->ActionType = RETURN_DATA|USER_DATA;
            ((MessageHeader*)Buffer)->StatusCode = htons(GET_DATA_SUCCESS);
            Length = _Package(Buffer,sizeof(MessageHeader)+sizeof(UserData),NORMAL_MESSAGE,Package);
            send(Sockfd,Package,Length,0);
            goto RETURN_SUCCESS;
        }
        goto RETURN_FAILURE;
    }else{
        LinkC_Debug("Data Returns: Operation Not Supported",LINKC_WARNING);
        goto RETURN_FAILURE;
    }
RETURN_SUCCESS:
    free(Buffer);
    free(Package);
    Buffer = NULL;
    Package = NULL;
    return LINKC_SUCCESS;
RETURN_FAILURE:
    free(Buffer);
    free(Package);
    Buffer = NULL;
    Package = NULL;
    return LINKC_FAILURE;
}
