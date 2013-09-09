#define MAXBUF 512
#include "NetWork/csocket.h"
#include "NetWork/p2p_client.h"
#include <stdio.h>

p2p_client::p2p_client(){
    this->Set_IP("117.59.12.19");
    this->Set_Port(2342);
}

p2p_client::~p2p_client(){

}

void p2p_client::SetDestIP(ip_t ip){
    info.DestIP=ip;
}

int p2p_client::inDirectConnect(){
    if (info.DestIP == 0){
        printf("Dest Ip is not set!\n");
        return -1;
    }
    int i = this->ConnectToServer();
    if (i != 1) return i;
    this->Send_msg((void *)&info,MAXBUF,MSG_WAITALL);
    return 1;
}
