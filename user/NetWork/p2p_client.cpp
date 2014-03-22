/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： March 22, 2014
 */
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
    printf("Debug >> P2P Connection\t= [STARTED]\n");
    this->start();
}

void p2p_client::run(){
    if (Dest.Recv_msg((void *)&P2PInfo,sizeof(P2PInfo),0) == 0){
        printf("Debug >> P2P Connection\t= [ADDRESS GOT FAILURE]\n");
        return;
    }
    Dest.SetAddress(P2PInfo.Dest);                         // 设置为好友的IP地址
    printf("Debug >> P2P Connection\t= [ADDRESS GOT]\n");
    Connecter = new P2PConnecter(Dest,P2PInfo);
    this->connect(Connecter,SIGNAL(ReadyToAccept()),this,SLOT(ReadyToAccept()));
    this->connect(Connecter,SIGNAL(ConnectToPeerDone(bool)),this,SLOT(ConnectDone(bool)));
    if(P2PInfo.is_server == 1){
        Connecter->start();
        LinkC_Debug("I Am Server!");
    }else{
        LinkC_Debug("I Am Client!");
    }
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

void p2p_client::ReadyToAccept(){
    emit AcceptReady();
}

void p2p_client::ConnectDone(bool k){
    emit ConnectToPeerDone(k);
    isPeerConnected = k;
}

void p2p_client::inDirectConnectStart(){
    Connecter->start();
}

P2PConnecter::P2PConnecter(UDP_csocket k,p2pinfo info, QThread *parent):
    QThread(parent){
    Dest = k;
    P2PInfo = info;
    package = new char[MAX_BUFFER_SIZE + STD_PACKAGE_SIZE + 1];
}

void P2PConnecter::run(){
    if(P2PInfo.is_server == 0){
        inDirectConnect();
    }else{
        inDirectAccept();
    }
}


void P2PConnecter::inDirectAccept(){
    int length;
    int flag;
    Dest.Send_msg("The packet will be discarded by NAT device",MSG_DONTWAIT);
    emit ReadyToAccept();
    bzero(buffer,STD_PACKAGE_SIZE);
    LinkC_Debug("Useless Message Sended!");
    Dest.Recv_msg(buffer,LMH_L,0);
    flag = get_message_header(buffer);
    if(flag != CONNECTION){
    printf("Debug >> P2P Connection\t= [FAILURE]\n");
        emit ConnectToPeerDone(false);
        return;
    }
    length = pack_message(CONNECTION,NULL,0,buffer);
    Dest.Send_msg(buffer,length,0);
    printf("Debug >> P2P Connection\t= [SUCCESS]\n");
    emit ConnectToPeerDone(true);
}

void P2PConnecter::inDirectConnect(){
    int length;
    int flag;
#ifdef _LOCAL_NETWORK_P2P_TEST_
    Dest.Recv_msg(buffer,LMH_L,0);      // 因为这里接收的数据在有NAT设备的情况下是不会到达的
    LinkC_Debug("Useless Message Recved!");
#endif
    length = pack_message(CONNECTION,NULL,0,buffer);
    Dest.Send_msg(buffer,length,0);     // 发送连接请求
    bzero (buffer,STD_PACKAGE_SIZE);
    Dest.Recv_msg(buffer,STD_PACKAGE_SIZE,0);
    flag = get_message_header(buffer);
    if(flag != CONNECTION){
    printf("Debug >> P2P Connection\t= [FAILURE]\n");
        emit ConnectToPeerDone(false);
        return;
    }
    printf("Debug >> P2P Connection\t= [SUCCESS]\n");
    emit ConnectToPeerDone(true);
}
