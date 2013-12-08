#include "Csocket.h"
#include "p2p_client.h"
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>

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
    if (this->Recv_msg((void *)&P2PInfo,sizeof(P2PInfo),0) == 0)
        return -1;
    struct timeval tv;
    tv.tv_sec = 6;                                          // 6 second timed out
    tv.tv_usec = 0;                                         // 0 mic second
    this->SetTimedOut(tv);
    int i;
    this->SetAddress(P2PInfo.Dest);
    for (i=0;i<5;i++)
    {
        strncpy(buffer,"1",1);
        buffer[1] = '\0';
        if(this->Send_msg(buffer,MAXBUF,MSG_DONTWAIT) <= 0)
        {
            perror("Send");
            continue;
        }
        strncpy(buffer,"0",1);
        if(this->Recv_msg(buffer,MAXBUF,0) <= 0)
        {
                perror("Recv");
                continue;
        }
        if(atoi(buffer)==1)
        {
            this->Send_msg(buffer,MAXBUF,MSG_DONTWAIT);
            break;
        }
        sleep(1);
    }
    printf("Done.\n");
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
