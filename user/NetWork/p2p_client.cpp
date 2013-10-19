#define MAXBUF 512
#include "NetWork/csocket.h"
#include "NetWork/p2p_client.h"
#include <stdio.h>
#include <netdb.h>

p2p_client::p2p_client(){
    build_socket(UDP);
    ip_size = sizeof(ip_t);
}

p2p_client::~p2p_client(){

}

int p2p_client::DirectConnect(){

    return 0;
}

int p2p_client::inDirectConnect(){

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
