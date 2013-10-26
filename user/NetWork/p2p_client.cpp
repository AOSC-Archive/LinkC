#include "csocket.h"
#include "p2p_client.h"
#include <stdio.h>
#include <netdb.h>

p2p_client::p2p_client(){
    ip_size = sizeof(ip_t);
}

p2p_client::~p2p_client(){

}

int p2p_client::DirectConnect(){

    return 0;
}

int p2p_client::inDirectConnect(){
    this->Send_msg((void *)&DestIP,ip_size,MSG_DONTWAIT);
    if (this->Recv_msg((void *)&P2PInfo,sizeof(P2PInfo),MSG_WAITALL) == 0)
        return -1;
    return 0;
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
