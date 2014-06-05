/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： April 4, 2014
 */
#include "MessageRecver.h"
#include "linkc_network_protocol.h"
#include "Protocol/LinkC_NetWorkProtocol.h"
#include "linkc_types.h"
#include <QMessageBox>
#include <iostream>
#include <stdio.h>
using namespace std;

TCP_MessageRecver::TCP_MessageRecver(class TCP_csocket *sk,QThread *parent):
    QThread(parent){
    Dest = *sk;

}

TCP_MessageRecver::~TCP_MessageRecver(){
}

void TCP_MessageRecver::run(){
    int header;
    void* buffer = malloc(MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
    void* package = malloc(MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
    int Totle;
    void *tmp;
    int i;
    LinkC_Debug("TCP MessageRecver",LINKC_STARTED);
    while(1){
        bzero(buffer,MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
        bzero(package,MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
        if(Dest.TCP_Recv(buffer,MAX_BUFFER_SIZE,0) == LINKC_FAILURE){
            emit RecvError();
            perror("TCP_Recv:");
            sleep(10);
            continue;
        }
        header = get_message_header(buffer);
        if(header > 0){
            Totle = ((LMH*)buffer)->Totle;
            unpack_message(buffer,package);
        }else   continue;

        if(header == USER_MESSAGE){         // 如果是好友信息
            emit UserMessage(*(LUM*)package);
            continue;
        }
        else if(header == SYS_ACTION_STATUS){
            emit SysActionStatus(*(LSS *)package);
            LinkC_Debug("Get Friends Data",((LSS *)package)->Status);
            if(((LSS*)package)->Action == USER_FRIENDS_DATA && ((LSS*)package)->Status == LINKC_SUCCESS){
                tmp = new char[(Totle)*sizeof(LinkC_Friend_Data)];    // 参见 server/src/network/network_protocol/send_friends_data.c[我TM太聪明了那段]
                for(i=0;i<Totle;i++){
                    Dest.TCP_Recv(buffer,STD_BUFFER_SIZE,0);
                    unpack_message(buffer,package);
                    memcpy((char *)tmp+(i*sizeof(LinkC_Friend_Data_t)),package,sizeof(LinkC_Friend_Data_t));
                }
                emit SysFriendsList(tmp,Totle);
            }
        }
        else if(header == SYS_FRIEND_DATA){
            LinkC_Debug("Get Friend Data",LINKC_SUCCESS);
            emit SysFriendData(*(LFD *)package);
        }
        else if(header == SYS_USER_INFO){
            LinkC_Debug("Get User Info",LINKC_SUCCESS);
            emit SysUserInfo(*(LUI *)package);
        }
        else
            LinkC_Debug("This Message is not supposed!",LINKC_WARNING);
    }
}

UDP_MessageRecver::UDP_MessageRecver(UDP_csocket sk, QThread *parent):
    QThread(parent){
    Dest = sk;
    buffer = malloc(MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
    package = malloc(MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
}
void UDP_MessageRecver::run(){
    int header;
    int MsgLength;
    QString Message;
    LinkC_Debug("UDP MessageRecver",LINKC_STARTED);
    while(1){
        bzero(buffer,MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
        bzero(package,MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
        if(Dest.UDP_Recv(buffer,MAX_BUFFER_SIZE,0) == LINKC_FAILURE){
            emit RecvError();
            perror("Recv:");
            continue;
        }
        header = get_message_header(buffer);
        unpack_message(buffer,package);
        if(header == USER_CHAT_MESSAGE){
            MsgLength = strlen((const char *)package);
            ((char *)package)[MsgLength] = 0;
            Message = (const char *)package;
            emit RecvedP2PMessage(Message);
        }/*else if(header == HEART_BEATS)
            emit HeartBeats();*/
        else
            LinkC_Debug("This Message is not supposed!",LINKC_WARNING);
    }
}
