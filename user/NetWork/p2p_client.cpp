#include "Csocket.h"
#include "p2p_client.h"
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>

p2p_client::p2p_client(QThread *parent):
    QThread(parent){
    ip_size = sizeof(ip_t);
    package = new char[MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1];
    isPeerConnected = false;
}

p2p_client::~p2p_client(){
}

int p2p_client::DirectConnect(){

    return 0;
}

int p2p_client::SendP2PMessage(char *ch){
    int length;
    if(ch == NULL)
        return -1;
    length = strlen(ch);
    if(length == 0)
        return -1;
    length = pack_message(USER_CHAT_MESSAGE,ch,length,buffer);
    Dest.Send_msg(buffer,0);
    return 0;
}

void p2p_client::ConnectToPeer(void){
    Dest.Set_IP(SERVER_IP);
    Dest.Debug_Csocket_IP();
    Dest.Set_Port(2342);
    Dest.Debug_Csocket_Port();

    Dest.Send_msg((void *)&DestIP,ip_size,0);
    printf("Waiting For peer's Address!\n");
    this->start();

}

void p2p_client::run(){
    if (Dest.Recv_msg((void *)&P2PInfo,sizeof(P2PInfo),0) == 0){
        return;
    }
    Dest.SetAddress(P2PInfo.Dest);                         // 设置为好友的IP地址
    printf("Get Peer's Address Success\n");
    if(P2PInfo.is_server == 1)
        this->inDirectAccept();
    else
        this->inDirectConnect();
}

void p2p_client::inDirectAccept(){
    int tmp;
    Dest.Send_msg("The packet will be discarded NAT device",MSG_DONTWAIT);
    bzero(buffer,STD_PACKAGE_SIZE);
    Dest.Recv_msg(buffer,LMH_L,MSG_WAITALL);
    flag = get_message_header(buffer);
    if(flag != CONNECTION){
        fprintf(stderr,"Message Header Increect!\n");
        emit ConnectToPeerDone(false);
        return;
    }
    tmp = pack_message(CONNECTION,NULL,0,buffer);
    Dest.Send_msg(buffer,tmp,0);
    printf("InDirectAccept Success!!!!!!!!!!!!!\n");
    isPeerConnected = true;
    emit ConnectToPeerDone(true);
}

void p2p_client::inDirectConnect(){
    int tmp;
    sleep(2);
    tmp = pack_message(CONNECTION,NULL,0,buffer);
    Dest.Send_msg(buffer,tmp,0);
    Dest.Recv_msg(buffer,LMH_L,MSG_WAITALL);

    bzero (buffer,STD_PACKAGE_SIZE);
    Dest.Recv_msg(buffer,STD_PACKAGE_SIZE,0);
    flag = get_message_header(buffer);
    if(flag != CONNECTION){
        fprintf(stderr,"Message Header Incrrect!\n");
        emit ConnectToPeerDone(false);
        return;
    }
    printf("InDirectConnect Success!!!!!!!!!!!!!\n");
    isPeerConnected = true;
    emit ConnectToPeerDone(true);
}

int p2p_client::IsServer(){
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

p2pinfo p2p_client::GetP2PInfo(){
    return P2PInfo;
}

bool p2p_client::IsPeerConnected(){
    return isPeerConnected;
}

UDP_csocket p2p_client::GetCsocket(){
    return Dest;
}

void p2p_client::SetPeerIP(ip_t ip){
    Dest.Set_IP(ip);
}
