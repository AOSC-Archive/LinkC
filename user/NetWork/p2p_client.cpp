/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： April 5, 2014
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
    delete [] (char *)package;
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
    Dest.Set_Port(2342);
    Dest.Send_msg((void *)&DestIP,ip_size,0);
    this->start();
}

void p2p_client::run(){
    int length;
    int flag;
    LinkC_Debug("P2P-->Connect",LINKC_STARTED);
    if (Dest.Recv_msg((void *)&P2PInfo,sizeof(P2PInfo),0) == 0){
        LinkC_Debug("P2P-->Get Address",LINKC_FAILURE);
        return;
    }
    Dest.SetAddress(P2PInfo.Dest);                         // 设置为好友的IP地址
    LinkC_Debug("P2P-->Get Address",LINKC_SUCCESS);
    if(P2PInfo.is_server == 1){     // Accept
        Dest.Send_msg("The packet will be discarded by NAT device",MSG_DONTWAIT);
        bzero(buffer,STD_PACKAGE_SIZE);
        LinkC_Debug("Useless Message Send",LINKC_DONE);
        emit ReadyToAccept();
        Dest.Recv_msg(buffer,LMH_L,0);
        flag = get_message_header(buffer);
        if(flag != CONNECTION){
            emit ConnectToPeerDone(false);
            return;
        }
        length = pack_message(CONNECTION,NULL,0,buffer);
        Dest.Send_msg(buffer,length,0);
        bzero(buffer,STD_PACKAGE_SIZE);
        Dest.Recv_msg(buffer,LMH_L,0);
        flag = get_message_header(buffer);
        if(flag != CONNECTION){
            isPeerConnected=false;
            emit ConnectToPeerDone(false);
            return;
        }
        emit ConnectToPeerDone(true);
    }
}

int p2p_client::IsServer(){
    return P2PInfo.is_server;
}

int p2p_client::SetDestIP(const char *ip){
    struct hostent *hp;
    hp=gethostbyname(ip);
    if(hp==NULL){
        LinkC_Debug("Get Host By Name",LINKC_FAILURE);
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

void p2p_client::inDirectConnectStart(){
    int length;
    int flag;
#if _LOCAL_NETWORK_P2P_TEST_
    Dest.Recv_msg(buffer,LMH_L,0);      // 因为这里接收的数据在有NAT设备的情况下是不会到达的
    LinkC_Debug("Useless Message Recved",LINKC_DONE);
#endif
    length = pack_message(CONNECTION,NULL,0,buffer);    // 第一次握手
    Dest.Send_msg(buffer,length,0);     // 发送连接请求
    bzero (buffer,STD_PACKAGE_SIZE);
    Dest.Recv_msg(buffer,STD_PACKAGE_SIZE,0);
    flag = get_message_header(buffer);
    if(flag != CONNECTION){
        isPeerConnected=false;
        emit ConnectToPeerDone(false);
        return;
    }
    length = pack_message(CONNECTION,NULL,0,buffer);    // 第二次握手
    Dest.Send_msg(buffer,length,0);     // 发送连接请求

    LinkC_Debug("P2P-->Connect",LINKC_SUCCESS);
    isPeerConnected=true;
    emit ConnectToPeerDone(true);
}
