#include "Csocket.h"
#include "p2p_client.h"
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>

p2p_client::p2p_client(){
    ip_size = sizeof(ip_t);
    package = new char[MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1];
    DestAddr.sin_family=AF_INET;
}

p2p_client::~p2p_client(){
}

int p2p_client::DirectConnect(){

    return 0;
}

int p2p_client::WaitPeer(){
    this->Send_msg((void *)&DestIP,ip_size,0);
    if (this->Recv_msg((void *)&P2PInfo,sizeof(P2PInfo),0) == 0){
        return LINKC_FAILURE;
    }
    struct timeval tv;
    tv.tv_sec = 6;                                          // 设置6妙超时
    tv.tv_usec = 0;                                         //
    this->SetTimedOut(tv);                                  //
    this->SetAddress(P2PInfo.Dest);                         // 设置为好友的IP地址
    printf("Wait Done\n");
    return LINKC_SUCCESS;
}

int p2p_client::inDirectAccept(){
    Send_msg("Any Thing You Want",MSG_DONTWAIT);
    sleep(1);
    bzero(buffer,STD_PACKAGE_SIZE);
    UDP_Recv(buffer,STD_PACKAGE_SIZE,0);
    flag = get_message_header(buffer);
    if(flag != CONNECTION){
        fprintf(stderr,"Message Header Increect!\n");
        return LINKC_FAILURE;
    }
    tmp = pack_message(CONNECTION,NULL,0,buffer);
    Send_msg(buffer,0);
    Send_msg(buffer,0);
    printf("InDirectAccept Success!!!!!!!!!!!!!\n");
    return LINKC_SUCCESS;
}

int p2p_client::inDirectConnect(){

    sleep(1);                                               // sleep 2 second
    tmp = pack_message(CONNECTION,NULL,0,buffer);
    Send_msg(buffer,tmp,0);
    sleep(1);
    Recv_msg(buffer,STD_PACKAGE_SIZE,0);

    bzero (buffer,STD_PACKAGE_SIZE);
    Recv_msg(buffer,STD_PACKAGE_SIZE,0);
    flag = get_message_header(buffer);
    if(flag != CONNECTION){
        fprintf(stderr,"Message Header Incrrect!\n");
        return -1;
    }
    printf("InDirectConnect Success!!!!!!!!!!!!!\n");
    return 0;
}

int p2p_client::Is_server(){
    return P2PInfo.is_server;
}

int p2p_client::SetDestIP(const char *ip){
    struct hostent *hp;
    hp=gethostbyname(ip);
    if(hp==NULL){
        printf("Get Host By Name Error\n");
        return -1;
    }
    memcpy (&DestIP,hp->h_addr,4);
    return 0;
}

int p2p_client::SetDestIP(ip_t ip){
    DestIP = ip;
    return 0;
}
