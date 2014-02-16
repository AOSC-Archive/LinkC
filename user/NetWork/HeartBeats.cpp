#include "HeartBeats.h"

HeartBeats::HeartBeats(int sockfd,QThread *parent):
    QThread(parent){
    ConnFd = sockfd;
    SendBuffer = new char[128];
}

void HeartBeats::run(){
    int length = pack_message(HEART_BEATS,NULL,0,SendBuffer);
    int status;
    while(1){
        status = send(ConnFd,SendBuffer,length,0);
        if(status <= 0)
            emit SendError(status);
        sleep(HEART_BEATS_TIME);
    }
}

void HeartBeats::HeartBeatsStop(){
    if(this->isRunning() == true)
        this->terminate();
    // Do Something
    delete SendBuffer;
}
