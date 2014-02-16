#include "MessageRecver.h"
#include "linkc_network_protocol.h"
#include "linkc_types.h"
#include <QMessageBox>

TCP_MessageRecver::TCP_MessageRecver(class TCP_csocket *sk,QThread *parent):
    QThread(parent){
    Dest = *sk;
    buffer = malloc(MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
   package = malloc(MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
}

TCP_MessageRecver::~TCP_MessageRecver(){
    free(buffer);
    free(package);
}

void TCP_MessageRecver::run(){
    int header;
    printf("TCP MessageRecver Started!\n");
    QVariant Variant;
    while(1){
        bzero(buffer,MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
        bzero(package,MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
        if(Dest.TCP_Recv(buffer,MAX_BUFFER_SIZE,0) == LINKC_FAILURE){
            emit RecvError();
            perror("Recv:");
            sleep(10);
            continue;
        }
        header = get_message_header(buffer);
        unpack_message(buffer,package);
        if(header == USER_MESSAGE){         // 如果是好友信息
//            emit UserMessage(*(LUM*)package);
            continue;
        }
        else if(header == SYS_ACTION_STATUS){
            Variant.setValue(*(LSS *)package);
            emit SysActionStatus(Variant);
        }
        else if(header == SYS_FRIEND_DATA){

        }
        else{
            fprintf(stderr,"This Message Is Not Supposed!\n");
        }
    }
}

UDP_MessageRecver::UDP_MessageRecver(UDP_csocket *sk, QThread *parent):
    QThread(parent){
    Dest = *sk;
    buffer = malloc(MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
    package = malloc(MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
}

void UDP_MessageRecver::run(){
    int header;
    printf("UDP MessageRecver Started!\n");
    QVariant Variant;
    QString Message;
    while(1){
        bzero(buffer,MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
        bzero(package,MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
        if(Dest.UDP_Recv(buffer,MAX_BUFFER_SIZE,0) == LINKC_FAILURE){
            emit RecvError();
            perror("Recv:");
            sleep(10);
            continue;
        }
        header = get_message_header(buffer);
        unpack_message(buffer,package);
        if(header == USER_CHAT_MESSAGE){

        }
        else{
            fprintf(stderr,"This Message Is Not Supposed!\n");
        }
    }
}
