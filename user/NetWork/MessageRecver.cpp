#include "MessageRecver.h"
#include "linkc_network_protocol.h"
#include "linkc_types.h"
#include <QMessageBox>
#include <iostream>
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
    printf("Debug >> TCP Recver\t= [STARTED]\n");
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
        if(header > 0)
            unpack_message(buffer,package);
        if(header == USER_MESSAGE){         // 如果是好友信息
            emit UserMessage(*(LUM*)package);
            continue;
        }
        else if(header == SYS_ACTION_STATUS){
            emit SysActionStatus(*(LSS *)package);
        }
        else if(header == SYS_FRIEND_DATA){
            emit SysFriendData(*(LFD *)package);
        }
        else{
            fprintf(stderr,"This Message Is Not Supposed!\n");
        }
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
    printf("Debug >> UDP Recver\t= [STARTED]\n");
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
        }else if(header == HEART_BEATS){
            emit HeartBeats();
        }
        else{
            fprintf(stderr,"This Message Is Not Supposed!\n");
        }
    }
}
